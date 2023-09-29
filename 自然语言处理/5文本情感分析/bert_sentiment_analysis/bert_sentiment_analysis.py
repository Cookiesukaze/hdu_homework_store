import pandas as pd
import torch
from torch.utils.data import DataLoader
from dataset import *
from bert_classifier import *
from train import *
'''
3.BERT情感分析
'''
df = pd.read_csv("流浪地球_preprocess_bert.csv", encoding="ANSI",header=None)
dataset = Dataset(df)

# —————————————————按8-1-1 划分数据集———————————————————
df_train, df_val, df_test = np.split(df.sample(frac=1, random_state=42),
                                     [int(.8 * len(df)), int(.9 * len(df))])

# —————————————————创建模型———————————————————
model = BertClassifier()
# —————————————————训练——————————————————
EPOCHS = 10
LR = 1e-6
train(model, df_train, df_val, LR, EPOCHS)
