import pandas as pd
from extract_words import *
import torch
from torch.utils.data import DataLoader
from dataset import *
from bert_classifier import *
from train import *

df = pd.read_csv("message80W_deWeight.csv", header=None)
dataset = Dataset(df)

# —————————————————按8-1-1 划分数据集———————————————————
df_train, df_val, df_test = np.split(df.sample(frac=1, random_state=42),
                                     [int(.8 * len(df)), int(.9 * len(df))])

# —————————————————创建模型———————————————————
model = BertClassifier()
# —————————————————训练——————————————————
EPOCHS = 5
LR = 1e-6
train(model, df_train, df_val, LR, EPOCHS)
