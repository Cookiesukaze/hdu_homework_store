from flask import Flask, request, jsonify, send_from_directory
from flask_cors import CORS
import cv2
import numpy as np
import time
import os
from sklearn.cluster import MiniBatchKMeans
from PIL import Image
from transformers import ViTImageProcessor, ViTForImageClassification
import torch
import io
from numba import jit, prange

# 设置环境变量以防止内存泄漏
os.environ["OMP_NUM_THREADS"] = "1"

# 加载预训练的 ViT 模型和图像处理器
model_name = "google/vit-base-patch16-224"
image_processor = ViTImageProcessor.from_pretrained(model_name)
model = ViTForImageClassification.from_pretrained(model_name)

# 创建 Flask 应用
app = Flask(__name__)
CORS(app)  # 启用 CORS

# 创建一个固定目录来存储图像
UPLOAD_FOLDER = 'static/images'
if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)

@jit(nopython=True, parallel=True)
def rgb_to_gray_numba(image):
    """使用 Numba JIT 编译器将 RGB 图像转换为灰度图像"""
    h, w, _ = image.shape
    gray_image = np.empty((h, w), dtype=np.uint8)
    for i in prange(h):
        for j in prange(w):
            r, g, b = image[i, j]
            gray_image[i, j] = 0.2989 * r + 0.5870 * g + 0.1140 * b
    return gray_image

def create_color_palette(colors, size=100):
    """创建颜色调色板"""
    palette = np.zeros((4 * size, 4 * size, 3), dtype=np.uint8)
    for i in range(4):
        for j in range(4):
            palette[i * size:(i + 1) * size, j * size:(j + 1) * size] = colors[i * 4 + j]
    return palette

def identify_image(image):
    """识别图像中的对象"""
    inputs = image_processor(images=image, return_tensors="pt")
    with torch.no_grad():
        outputs = model(**inputs)
    logits = outputs.logits
    probabilities = torch.nn.functional.softmax(logits, dim=-1)[0]
    top_5 = torch.topk(probabilities, 5)

    results = []
    for i in range(5):
        label = model.config.id2label[top_5.indices[i].item()]
        score = top_5.values[i].item()
        results.append({"label": label, "score": score})

    return results

def plot_histogram_cv2(hist, hist_path):
    """使用 OpenCV 绘制直方图并保存为 256x100 像素"""
    hist_image = np.zeros((100, 256, 3), dtype=np.uint8)
    cv2.normalize(hist, hist, alpha=0, beta=100, norm_type=cv2.NORM_MINMAX)
    for x in range(256):
        cv2.line(hist_image, (x, 100), (x, 100 - int(hist[x])), (255, 255, 255))
    hist_image = cv2.resize(hist_image, (256, 100))
    cv2.imwrite(hist_path, hist_image)

def process_histogram(image_np, hist_path):
    """处理图像，生成直方图并记录每个步骤的时间"""
    # 记录每个步骤的时间
    times = {}

    start_time = time.time()
    gray_image = rgb_to_gray_numba(image_np)  # 使用 Numba 并行进行灰度转换
    times['gray_conversion'] = time.time() - start_time

    start_time = time.time()
    hist = cv2.calcHist([gray_image], [0], None, [256], [0, 256])
    cv2.normalize(hist, hist)  # 归一化直方图
    hist = hist.flatten()
    times['histogram_computation'] = time.time() - start_time

    start_time = time.time()
    plot_histogram_cv2(hist, hist_path)
    times['histogram_plotting'] = time.time() - start_time

    return times

def analyze_image(image_data):
    """分析上传的图像"""
    image = Image.open(io.BytesIO(image_data))
    image_np = np.array(image)

    overall_start_time = time.time()
    times = {}

    # 0. 图像分类
    start_time = time.time()
    top_5_classes = identify_image(image)
    times['classification'] = time.time() - start_time

    # 1. 直方图
    start_time = time.time()
    histogram_path = os.path.join(UPLOAD_FOLDER, 'histogram.png')

    # 计算直方图
    hist_times = process_histogram(image_np, histogram_path)
    times.update(hist_times)
    times['histogram'] = time.time() - start_time

    # 2. 边缘检测
    edges_start_time = time.time()
    gray_image = cv2.cvtColor(image_np, cv2.COLOR_RGB2GRAY)
    edges = cv2.Canny(gray_image, 100, 200)
    edges_resized = cv2.resize(edges, (400, 400))  # 统一为 400x400 像素
    edges_path = os.path.join(UPLOAD_FOLDER, 'edges.png')
    cv2.imwrite(edges_path, edges_resized)
    times['edge_detection'] = time.time() - edges_start_time

    # 3. 主颜色提取（16种颜色）
    start_time = time.time()
    small_image = cv2.resize(image_np, (100, 100))
    pixels = small_image.reshape((-1, 3))
    pixels = np.float32(pixels)
    kmeans = MiniBatchKMeans(n_clusters=16, batch_size=6144, max_iter=10, n_init='auto')
    kmeans.fit(pixels)
    colors = kmeans.cluster_centers_  # 注意这里的下划线
    color_palette = create_color_palette(colors, size=25)  # 4x4 色块，加起来为 400x400
    palette_path = os.path.join(UPLOAD_FOLDER, 'palette.png')
    cv2.imwrite(palette_path, color_palette)
    times['main_colors'] = time.time() - start_time

    # 计算总体耗时
    overall_elapsed_time = time.time() - overall_start_time
    times['overall'] = overall_elapsed_time

    timing_output = {k: f"{v:.3f} seconds" for k, v in times.items()}

    # 打印分类结果和处理时间
    print("分类结果:", top_5_classes)
    print("处理时间:", timing_output)

    return top_5_classes, 'histogram.png', 'edges.png', 'palette.png', timing_output

@app.route('/spawn', methods=['POST'])
def spawn():
    """处理上传的图像并返回分析结果"""
    # 获取上传的图像数据
    image_data = request.files['image'].read()

    # 调用分析图像的函数
    top_5_classes, histogram_path, edges_path, palette_path, timing_output = analyze_image(image_data)

    # 构建响应数据
    response_data = {
        'top_5_classes': top_5_classes,
        'histogram_path': histogram_path,
        'edges_path': edges_path,
        'palette_path': palette_path,
        'timing_output': timing_output
    }

    # 返回 JSON 响应
    return jsonify(response_data)

@app.route('/image/<filename>', methods=['GET'])
def get_image(filename):
    """返回指定路径的图像"""
    return send_from_directory(UPLOAD_FOLDER, filename)

if __name__ == "__main__":
    # 运行 Flask 应用
    app.run(host="0.0.0.0", port=8080)
