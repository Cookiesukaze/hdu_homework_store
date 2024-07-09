### 2024短学期-软件开发实践3

第三个短学期是做小程序，选了虹软合作的题目：直方图绘制 <br />
多做了几个功能，所以实际上是“图片分析”小程序 <br />
在此放出flask后端代码（app.py），可直接调用，上传一张图片即可 <br />
在 https://huggingface.co/spaces/Cookiesukaze/ImgAnalysis 在线体验 <br />

#### 功能

1.直方图绘制（选取了性能比较好的几个常用函数，生成速度快）：使用Numba的并行将图片转直方图、使用OpenCV进行直方图计算和绘制 <br />
2.图片分类：调用huggingface上的google/vit进行分类 <br />
3.边缘检测 <br />
4.Top16颜色卡片生成：涂鸦爱好者狂喜 <br />
5.用时统计：以上几个功能的用时 <br />
