# 构建LVCSR解码图/LVCSR解码优化

## 作业说明
本次作业依赖于lab1,需要对lab2的viterbi算法进行优化，减小时空开销

### p1
* 熟悉openfst的操作,
* 完成lab4.txt中的part1

### p2 
* 完成lab4.txt中的part2
* 构建hclg解码图
* 构建为word间增加sil的fst
* 了解tree文件，掌握state-tying cdphone和gmm tree的对应关系

### p3
* 实现不用存储完整chart的viterbi
* 处理skip arc
* 实现token pass版本的viterbi

### p4
* 增加beam pruning
* 增加rank pruning
* lab4_p4c.sh之后的作业需要attila库，不用做
