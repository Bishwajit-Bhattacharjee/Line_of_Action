mkfifo pipe
g++ -O3 AI.cpp -o AI
python main.py 2 < pipe | ./AI 8 8 1 > pipe
rm pipe