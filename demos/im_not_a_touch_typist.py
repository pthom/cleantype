# python im_not_a_touch_typist demo_cling.cpp | tee /dev/tty | cling --std=c++14

import sys
import time
import subprocess

def send_letter(letter):
  sys.stdout.write(letter)
  sys.stdout.flush()

def write_line(line):
  delay_letter = 0.040
  delay_line = 0.5
  for c in line:
    send_letter(c)
    time.sleep(delay_letter)
  time.sleep(delay_line)
  send_letter("\n")

def simulate_keypresses(content):
  lines = content.split("\n")
  for line in lines:
      write_line(line)


if __name__ == "__main__":
  filename = sys.argv[1]
  with open(filename, "r") as f:
    content = f.read()
  simulate_keypresses(content)
