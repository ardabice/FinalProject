# FinalProject
# 🧠 Memory and Disk Management Simulation
This repository contains C-based simulations of fundamental Operating System concepts, focusing on Disk Space Allocation and Memory Management Algorithms. The project was developed as part of the Operating Systems course at Istanbul Aydin University.

## 📁 Project Structure
The repository includes the following core source files:

ai_generated_version.c  
Initial skeleton implementation providing basic logic for bitmap allocation and linked-list based disk allocation.

arda_bice_version.c  
The final and enhanced version of the disk allocation simulation. It includes full Bitmap Allocation logic and the following experiments: Experiment 1: Speed test for 100 allocation operations. Experiment 2: Fragmentation Analysis with a 12-unit allocation request. Experiment 3: Allocation Trace Simulation following a predefined sequence.

memory_management.c  
Simulation of memory segment search algorithms using linked lists. It implements Best Fit (smallest sufficient block), Worst Fit (largest available block), and Next Fit (search continues from the last allocation point).

## 🚀 How to Run
Make sure you have a C compiler installed (e.g. gcc).

Disk Management Simulation:
gcc arda_bice_version.c -o arda_test
./arda_test

Memory Management Simulation:
gcc memory_management.c -o mem_test
./mem_test

## 📊 Key Experiments & Results
Disk Management (Question 2): The Fragmentation Test demonstrates that bitmap allocation requires consecutive free blocks. In the simulation, the 12-unit allocation request failed even though total free space was sufficient, due to fragmentation into small non-contiguous holes.

Memory Management (Question 3): Best Fit finds tight gaps but causes high fragmentation. Worst Fit prioritizes keeping large free blocks by splitting the biggest segments. Next Fit improves allocation speed by starting from the last successful allocation point.

## 🛠 Reproducibility
All experiments are fully reproducible. Fixed allocation sequences (traces) are used to simulate real-world operating system behavior under stress conditions.

## 👤 Author
Arda Bice  
Istanbul Aydin University  
Operating Systems
EOF

