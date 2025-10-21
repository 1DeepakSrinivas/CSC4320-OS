import matplotlib.pyplot as plt
import numpy as np
import os

def read_gantt_data(filename):
    if not os.path.exists(filename):
        raise FileNotFoundError(f"Gantt data file '{filename}' not found. Run the Round Robin program first.")
    
    processes = []
    start_times = []
    end_times = []
    
    with open(filename, 'r') as f:
        for line in f:
            try:
                pid, start, end = map(int, line.strip().split(','))
                processes.append(f'P{pid}')
                start_times.append(start)
                end_times.append(end)
            except ValueError as e:
                print(f"Warning: Skipping invalid line in {filename}: {line.strip()}")
                continue
    
    if not processes:
        raise ValueError("No valid data found in the Gantt data file")
    
    return processes, start_times, end_times

def plot_gantt_chart(filename):
    try:
        processes, starts, ends = read_gantt_data(filename)
        
        # Calculate dimensions
        num_processes = len(processes)
        max_time = max(ends)
        
        # Create figure with appropriate size
        fig_width = min(12, max(8, max_time/2))
        fig_height = min(6, max(3, num_processes/2))
        fig, ax = plt.subplots(figsize=(fig_width, fig_height))
        
        # Plot each process slot
        for i, (proc, start, end) in enumerate(zip(processes, starts, ends)):
            duration = end - start
            ax.barh(y=i, width=duration, left=start, 
                   height=0.5, color='lightblue', edgecolor='black')
            
            # Add process label
            ax.text(start + duration/2, i, proc, 
                   ha='center', va='center', fontsize=10)
        
        # Customize the plot
        ax.set_xlabel('Time Units')
        ax.set_ylabel('Processes')
        ax.set_title('Round Robin Scheduling Gantt Chart')
        
        # Set y-axis ticks and grid
        ax.set_yticks(range(num_processes))
        ax.set_yticklabels(processes)
        
        # Set x-axis grid and ticks
        all_times = sorted(list(set(starts + ends)))
        ax.set_xticks(all_times)
        ax.grid(axis='x', linestyle='--', alpha=0.7)
        
        # Add time labels at the top
        ax2 = ax.twiny()
        ax2.set_xlim(ax.get_xlim())
        ax2.set_xticks(all_times)
        ax2.set_xticklabels(all_times)
        
        plt.tight_layout()
        plt.savefig('gantt_chart.png', dpi=300, bbox_inches='tight')
        print(f"Gantt chart has been saved as 'gantt_chart.png'")
        plt.close()
        
    except Exception as e:
        print(f"Error: {str(e)}")
        return

if __name__ == "__main__":
    plot_gantt_chart("gantt_data.txt")