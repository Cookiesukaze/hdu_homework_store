import csv

import pandas as pd
from pyecharts.charts import Line
from pyecharts import options as opts
from sklearn.metrics import mean_squared_error

'''
4.对基于词典的情感分析结果与真实评分比较
'''

# —————————————————取出bosonNLP结果———————————————————
with open('bosonNLP_result.txt', 'r') as f:
    scores = f.readlines()
bosonNLP_result = []
for line in scores:
    bosonNLP_result.append(line.strip())
# print(bosonNLP_result)
# —————————————————取出真实评分———————————————————
df = pd.read_csv('流浪地球_preprocess.csv', encoding="ANSI")
true_result = df['scores'].tolist()
true_result = [x // 10 for x in true_result]  # 整除
# print(true_result)
# print(len(bosonNLP_result))
# print(len(true_result))
# —————————————————画图———————————————————
data1 = [opts.LineItem(name="基于词典的情感分析结果", value=value) for value in bosonNLP_result[:20]]
data2 = [opts.LineItem(name="真实评分", value=value) for value in true_result[:20]]
line = Line()
# x_data = list(range(1, 267))
x_data = list(range(1, 21))
line.add_xaxis(x_data)
line.add_yaxis("基于词典的情感分析结果", data1)
line.add_yaxis("真实评分", data2)
line.set_global_opts(
    title_opts=opts.TitleOpts(title="豆瓣影评文本情感分析-对基于词典的情感分析结果与真实评分部分比较"),
    legend_opts=opts.LegendOpts(
        orient="horizontal",
        pos_left="center",
        pos_top="90%"
    ),
)
line.render("豆瓣影评文本情感分析-对基于词典的情感分析结果与真实评分比较图.html", width="1800px")
# —————————————————误差计算———————————————————
true_result = [float(val) for val in true_result]
bosonNLP_result = [float(val) for val in bosonNLP_result]
mse = mean_squared_error(true_result, bosonNLP_result)
rmse = mse ** 0.5
print("基于词典的情感分析结果误差：")
print("MSE:", mse)
print("RMSE:", rmse)


# —————————————————统计———————————————————
def count_pairs_difference(y_true, y_pred, difference):
    count = 0
    for true, pred in zip(y_true, y_pred):
        if abs(true - pred) == difference:
            count += 1
    return count


difference_1_count = count_pairs_difference(true_result, bosonNLP_result, 1)
difference_2_count = count_pairs_difference(true_result, bosonNLP_result, 2)
print("基于词典的情感分析有{}对比较结果，其中：".format(len(true_result)))
print("相差1分的对数:", difference_1_count)
print("相差2分的对数:", difference_2_count)
print("相差3分及以上的对数:", len(true_result) - difference_1_count - difference_2_count)
