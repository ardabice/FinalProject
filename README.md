# FinalProject


Memory and Disk Management Simulation
This repository contains C implementations for simulating core Operating System concepts: Disk Space Allocation and Memory Management Algorithms. It was developed as part of the Operating Systems course at Istanbul Aydin University.
📁 Project Structure
The repository consists of the following key source files:
ai_generated_version.c: The initial skeleton code for bitmap and linked-list allocation logic.
arda_bice_version.c: The final, enhanced version of the disk allocation simulation. It includes:
Bitmap Allocation Logic.
Experiment 1: Speed Test for 100 allocations.
Experiment 2: Fragmentation Analysis with a 12-unit request.
+1
Experiment 3: Allocation Trace following a specific sequence.
+2
memory_management.c: Simulation of memory segment search algorithms using linked lists. It implements:
Best Fit: Scans the list for the smallest sufficient block.
Worst Fit: Selects the largest available block.
Next Fit: Searches from the last allocation point.
🚀 How to Run
To compile and run these simulations, you will need a C compiler (like gcc). Use the following commands in your terminal:
1. Disk Management Simulation
Bash
gcc arda_bice_version.c -o arda_test
./arda_test

2. Memory Management Simulation
Bash
gcc memory_management.c -o mem_test
./mem_test

📊 Key Experiments & Results
Disk Management (Question 2)
The Fragmentation Test demonstrates how bitmap allocation requires consecutive blocks. In the simulation, the 12-unit request FAILED because the disk was fragmented into small holes, even though total free space was sufficient.
Memory Management (Question 3)
Best Fit: Successfully found tightest gaps but caused high fragmentation.
Worst Fit: Prioritized keeping large leftovers by splitting the biggest blocks.
Next Fit: Optimized search speed by beginning from the last successful allocation point.
🛠 Reproducibility
All experiments are reproducible. The simulations use fixed sequences (Traces) to demonstrate real-world OS behavior under stress conditions.

Author: Arda Bice
University: Istanbul Aydin University
Course: Operating Systems
