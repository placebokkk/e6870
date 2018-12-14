# 实验1-MFCC特征提取和DTW


### 任务0:
安装boost库

mac上 `brew install boost`

`data/`目录下提供一组数据用于验证特征提取算法实现的正确性。
```
p1in.dat    一段原始音频信号
p1win.dat   分帧加窗后的信号 
p1fft.dat   每一帧fft变换
p1bin.dat   每一帧加mel-filter
p1all.dat   每一帧做DCT
```

请了解*.dat的数据格式，util.H中提供了读取和写入该格式的接口。


### 任务1:
完成front_end.C中三个BEGIN_LAB/END_LAB之间的代码

make lab1_frontend
./run_frontend.sh

### 任务2:
完成lab1_dtw.C中BEGIN_LAB/END_LAB之间的代码

make lab1_dtw
./run_dtw.sh
