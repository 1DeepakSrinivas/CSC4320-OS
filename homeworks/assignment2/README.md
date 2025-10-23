# CSC 4320 Homework 2 - CPU Scheduling Simulation
### Round Robin and Multilevel Feedback Queue Scheduling Algorithms

## Files

- `round_robin.c` - Round Robin scheduling implementation
- `mlfq.c` - Multilevel Feedback Queue scheduling implementation
- `main.sh` - Shell script to run both algorithms
- `plots.py` - Python script to generate Gantt chart visualizations
- `processes.txt` - Input file containing process data

## Prerequisites

- GCC compiler
- Python 3 with matplotlib

## How to Run

### Quick Start
```bash
chmod +x main.sh
./main.sh
```

This will:
1. Compile both C programs
2. Run Round Robin scheduling
3. Run MLFQ scheduling
4. Display performance comparison
5. Generate Gantt chart visualizations (if Python/matplotlib is installed)

## Output

- Terminal output with Gantt charts and performance metrics
- PNG files: `round_robin.png`, `mlfq.png`, `comparison.png`
- Data files: `round_robin.txt`, `mlfq.txt`

