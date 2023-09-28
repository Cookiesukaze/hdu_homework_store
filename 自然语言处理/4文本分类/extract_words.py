import jieba
import pandas as pd
import numpy as np


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
    df = pd.read_csv(csv_path, header=None, encoding='utf-8-sig')
    return df


# 分词并写入新的CSV文件
def process_and_save_csv(df, column_name, stop_word_path, only_vital_chinese, output_csv_path):
    df_copy = df.copy()  # 复制DataFrame
    df_copy.iloc[:, column_name]= df_copy.iloc[:, column_name].apply(
        lambda x: " ".join(divide_text(x, stop_word_path, only_vital_chinese)))
    df_copy.to_csv(output_csv_path, index=False, header=None, encoding='utf-8-sig')


# 测试
def extract_words_func(csv_path,column_name,stop_word_path,only_vital_chinese,output_csv_path):
    # csv_path = "test.csv"  # 短信文件路径
    # column_name = 2  # 待分词列
    # stop_word_path = "stopword.txt"  # 自定义分割词txt
    # only_vital_chinese = False  # 是否去掉分割词txt中的词
    # output_csv_path = "output_file_test.csv"  # 输出

    df = read_csv(csv_path)
    process_and_save_csv(df, column_name, stop_word_path, only_vital_chinese, output_csv_path)
# print(len(df_train),len(df_val), len(df_test))
# print(df)