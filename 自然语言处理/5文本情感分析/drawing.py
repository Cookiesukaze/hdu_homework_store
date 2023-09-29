import csv

import pandas as pd
from tools import *
from pyecharts.charts import Map
from pyecharts import options as opts
import random
from pyecharts.charts import Pie
from pyecharts.charts import Grid
from pyecharts.charts import Bar, Line
from extract_high_freq_words import *
from wordcloud import WordCloud
import matplotlib.pyplot as plt
import os

'''
2.数据可视化
'''
# —————————————————读取———————————————————
df = pd.read_csv("流浪地球_preprocess.csv", encoding='ANSI')
df_copy = df.copy()
# —————————————————创建字典———————————————————
df_evaluate = df_copy['evaluate'].tolist()
df_evaluate_dict = dict_creator(df_evaluate)
df_citys = df_copy['citys'].tolist()
df_citys_dict = dict_creator(df_citys)
# —————————————————画图-评论地区中国地图———————————————————
df_citys_keys = list(df_citys_dict.keys())
df_citys_values = []
for key, value in zip(df_citys_dict.keys(), df_citys_dict.values()):
    df_citys_values.append((key, value))
province = df_citys_keys
data_province = df_citys_values
china_province = (
    Map()
    .add('', data_province, 'china')
    .set_global_opts(
        title_opts=opts.TitleOpts(title='豆瓣影评文本情感分析-中国地图分布'),
        visualmap_opts=opts.VisualMapOpts(
            min_=min(df_citys_dict.values()),
            max_=max(df_citys_dict.values()),
            is_piecewise=False)
    )
    .render(path='豆瓣影评文本情感分析-中国地图分布.html')
)
# —————————————————画图-评论地区饼图———————————————————
pie = Pie()
pie.add("", data_province)
pie.set_global_opts(
    title_opts=opts.TitleOpts(title="豆瓣影评文本情感分析-评论地区", ),
    legend_opts=opts.LegendOpts(
        orient="horizontal",
        pos_left="center",
        pos_top="90%"
    ),
)
pie.set_series_opts(
    radius=["40%", "60%"],
)
pie.render('豆瓣影评文本情感分析-评论地区饼图.html', width="900px", height="900px")
# —————————————————画图-评分柱状图———————————————————
score_mapping = {'力荐': 5, '推荐': 4, '还行': 3, '较差': 2, '很差': 1}
key_list = ['力荐', '推荐', '还行', '较差', '很差']
df_evaluate_dict = dict(sorted(df_evaluate_dict.items(), key=lambda x: key_list.index(x[0])))  # 字典重排序
df_evaluate_scores = list(df_evaluate_dict.keys())
df_evaluate_counts = list(df_evaluate_dict.values())
total_score = sum(score_mapping[score] * count for score, count in df_evaluate_dict.items())
average_score = total_score / sum(df_evaluate_counts)
bar = (
    Bar()
    .add_xaxis(df_evaluate_scores)
    .add_yaxis("人数", df_evaluate_counts)
    .set_global_opts(
        title_opts=opts.TitleOpts(title="豆瓣影评文本情感分析-评分和人数（平均分：{:.2f}）".format(average_score)),
        yaxis_opts=opts.AxisOpts(name="人数"),
    )
    .set_series_opts(label_opts=opts.LabelOpts(position="inside"))
)
bar.render("豆瓣影评文本情感分析-评分和人数柱状图.html")
# —————————————————评论内容关键词提取———————————————————
csv_file = "流浪地球_preprocess.csv"  # 先把评论列提取成一个txt就可以调作业2了
column_name = "content"
txt_file = "流浪地球_preprocess_content.txt"
with open(csv_file, "r", newline="") as file:
    reader = csv.DictReader(file)
    column_data = [row[column_name] for row in reader]
with open(txt_file, "w") as file:
    for data in column_data:
        file.write(data + "\n")
divided_text = divide_text(text_path="流浪地球_preprocess_content.txt", stop_word_path="stoplist.txt",
                           only_vital_chinese=True)
wordcloud_dict = statistics_func(divided_text)
# —————————————————评论内容关键词词云绘制———————————————————
# print(wordcloud_dict)
font_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'fonts', 'SimHei.ttf')
wordcloud = WordCloud(font_path=font_path, background_color='white', scale=32, ).generate_from_frequencies(
    wordcloud_dict)
plt.imshow(wordcloud, interpolation='bilinear')
plt.axis('off')
# plt.show()
wordcloud.to_file('豆瓣影评文本情感分析-评论词云.png')

print("可视化完成")
