import jieba


# 分词
def divide_text(text_path, stop_word_path, only_vital_chinese):
    text = open(text_path, encoding='ANSI').read()
    stop_word = open(stop_word_path, encoding='UTF-8').read().splitlines()
    jieba.load_userdict(stop_word)  # 添加分词词典txt
    words = jieba.lcut(text)  # 分词并返回列表
    if only_vital_chinese:
        words = [word for word in words if word not in stop_word]  # 去掉stop_word里面的词
        words = [word for word in words if word != '\n']  # 去掉换行符
        words = [word for word in words if word != ' ']  # 新增-去掉空格
    # print(words)
    return words


# 排序
def statistics_func(text_list):
    dictionary = {}  # 创建字典
    for i in text_list:
        if i not in dictionary:
            dictionary[i] = 1
        else:
            dictionary[i] += 1
    # print(dictionary)
    sorted_dic = dict(sorted(dictionary.items(), key=lambda x: x[1], reverse=True))  # 按值倒序排序并重新生成字典
    # print(sorted_dic)
    for k, v in list(sorted_dic.items())[:30]:  # 转列表输出前三十项
        print(k, v)

