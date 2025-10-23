import matplotlib.pyplot as plt
import os

def read_gantt_data(filename):
    if not os.path.exists(filename):
        raise FileNotFoundError(f"File '{filename}' not found.")
    
    processes = []
    start_times = []
    end_times = []
    
    with open(filename, 'r') as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            try:
                pid, start, end = map(int, line.split(','))
                processes.append(f'P{pid}')
                start_times.append(start)
                end_times.append(end)
            except ValueError:
                continue
    
    if not processes:
        raise ValueError("No valid data found")
    
    return processes, start_times, end_times

def plot_gantt_chart(filename, title, output_filename):
    try:
        processes, starts, ends = read_gantt_data(filename)
        
        fig, ax = plt.subplots(figsize=(12, 6))
        
        for i, (proc, start, end) in enumerate(zip(processes, starts, ends)):
            duration = end - start
            ax.barh(i, duration, left=start, height=0.5, 
                   color='lightblue', edgecolor='black')
            ax.text(start + duration/2, i, proc, 
                   ha='center', va='center')
        
        ax.set_xlabel('Time Units')
        ax.set_ylabel('Processes')
        ax.set_title(title)
        ax.set_yticks(range(len(processes)))
        ax.set_yticklabels(processes)
        ax.grid(axis='x', alpha=0.3)
        
        plt.tight_layout()
        plt.savefig(output_filename, dpi=300, bbox_inches='tight')
        plt.close()
        
    except Exception as e:
        print(f"Error: {e}")

def plot_algos():
    try:
        plot_gantt_chart("mlfq.txt", "Multilevel Feedback Queue (MLFQ) Scheduling", "mlfq.png")
        plot_gantt_chart("round_robin.txt", "Round Robin Scheduling", "round_robin.png")
        
    except Exception as e:
        print(f"Error: {e}")

if __name__ == "__main__":
    plot_algos()