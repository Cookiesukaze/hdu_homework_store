import pandas as pd
from citys_preprocess import *
from tools import *

pd.set_option('display.max_columns', None)
pd.set_option('display.max_rows', None)
pd.set_option('max_colwidth', 100)
'''
1.预处理
本来想留下用户注册时间，想了想没啥用还是删掉了
而且仔细一看evaluate和score的五个等级其实是对应的，随便留哪个都行，选不出来所以都留下了
'''
# —————————————————读取———————————————————
df = pd.read_csv("流浪地球.csv", encoding='ANSI')
df_copy = df.copy()
# —————————————————预处理-对是否看过labs———————————————————
df_labs = df_copy['labs'].tolist()
df_labs_dict = dict_creator(df_labs)  # 统计发现{'看过': 480}，可去掉
df_copy = df_copy.drop(columns='labs')
# —————————————————预处理-删去nams和times和user_info———————————————————
df_copy = df_copy.drop(columns='nams')
df_copy = df_copy.drop(columns='times')
df_copy = df_copy.drop(columns='user_info')
# —————————————————预处理-对评估evaluate———————————————————
df_evaluate = df_copy['evaluate'].tolist()
df_evaluate_dict = dict_creator(df_evaluate)  # 统计发现出现了一些奇怪的时间作为evaluate，去掉
evaluate_list = {'推荐', '力荐', '还行', '较差', '很差'}
del_list = []
for index, e in enumerate(df_evaluate):
    if e not in evaluate_list:
        del_list.append(index)
df_copy = df_copy.drop(df_copy.index[del_list])
# —————————————————预处理-对评论content———————————————————
df_content = df_copy['content'].tolist()
del_list = []
for index, c in enumerate(df_evaluate):  # 无空评论
    if c is None:
        del_list.append(index)
# —————————————————预处理-对评分scores———————————————————
df_scores = df_copy['scores'].tolist()
df_scores_dict = dict_creator(df_scores)  # 无空评分
temp = []  # 评分转数字
for s in df_scores:
    s = s.strip("'[]")
    s = int(s)
    temp.append(s)
df_scores = temp
df_copy['scores'] = df_scores
# —————————————————预处理-对地区citys———————————————————
df_citys = citys_preprocess_func(df_copy)
df_citys_dict = dict_creator(df_citys)
df_copy['citys'] = df_citys
# —————————————————保存———————————————————
df_copy.to_csv("流浪地球_preprocess.csv", encoding='ANSI', index=False)
print("预处理完成")
