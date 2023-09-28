import jieba
import pandas as pd


# 分词
def divide_text(text, stop_word_path, only_vital_chinese):
    stop_word = open(stop_word_path, encoding='ANSI').read().splitlines()
    jieba.load_userdict(stop_word)  # 添加分词词典txt
    words = jieba.lcut(text)  # 分词并返回列表
    if only_vital_chinese:
        words = [word for word in words if word not in stop_word]  # 去掉stop_word里面的词
        words = [word for word in words if word != '\n']  # 去掉换行符
    # print(words)
    return words


# 读取CSV文件
def read_csv(csv_path):
    df = pd.read_csv(csv_path, header=None)
    return df


# 分词并写入新的CSV文件
def process_and_save_csv(df, column_name, stop_word_path, only_vital_chinese, output_csv_path):
    df_copy = df.copy()  # 复制DataFrame
    df_copy['分词结果'] = df_copy.iloc[:, column_name].apply(
        lambda x: " ".join(divide_text(x, stop_word_path, only_vital_chinese)))
    df_copy.to_csv(output_csv_path, index=False)


# 测试
csv_path = "message80W.csv"  # 替换为你的CSV文件路径
column_name = 2  # 替换为你的列名
stop_word_path = "stopword.txt"  # 替换为你的停用词文件路径
only_vital_chinese = True  # 是否仅保留中文关键词
output_csv_path = "output_file.csv"  # 新的CSV文件保存路径

df = read_csv(csv_path)
process_and_save_csv(df, column_name, stop_word_path, only_vital_chinese, output_csv_path)

