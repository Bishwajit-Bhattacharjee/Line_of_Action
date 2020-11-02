mkfifo pipe
g++ -O3 AI.cpp -o AI
python main.py 1 < pipe | ./AI 6 6 2 > pipe
rm pipe