mkfifo pipe
g++ AI.cpp -o AI
python main.py 1 < pipe | ./AI 6 6 2 | tee pipe
rm pipe