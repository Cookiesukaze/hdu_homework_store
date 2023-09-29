import pandas as pd
from tools import *

df = pd.read_csv("流浪地球_preprocess_bert.csv", encoding="ANSI",header=None)
df_s = df.iloc[:, 5].tolist()
df_s = dict_creator(df_s)
print(df_s)