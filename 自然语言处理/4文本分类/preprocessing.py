import pandas as pd
from extract_words import *

# —————————————————读取———————————————————
# df = pd.read_csv("message80W.csv", header=None)
df = pd.read_csv("test.csv", header=None)
# print(df.columns)
# print(df.head())
# print(len(df))
# 800000条
# print(df.iloc[:,1])
# —————————————————预处理-去重———————————————————
df = df.drop_duplicates(subset=[2])

df_copy = df.copy()
df_copy.to_csv("message80W_deWeight.csv", index=False, header=None, encoding='utf-8-sig')
# print(df.head())
# print(len(df))
# 786575条
# —————————————————预处理-分词———————————————————
# extract_words_func(csv_path="message80W_deWeight.csv",column_name=2,stop_word_path="stopword.txt",only_vital_chinese=True,output_csv_path= "message80W_preprocess.csv")
print("预处理完成")
