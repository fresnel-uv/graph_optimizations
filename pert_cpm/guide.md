# Project Scheduling Tool

This tool helps with project scheduling using **Critical Path Method (CPM)** and **Program Evaluation Review Technique (PERT)**. It calculates the critical path, task durations, and slack times for project tasks, and generates visualizations like Gantt charts and dependency graphs.

## Features
- **Critical Path Calculation**: Determines the longest sequence of dependent tasks.
- **Task Attributes**: Calculates Early Start (ES), Early Finish (EF), Late Start (LS), Late Finish (LF), and Slack Time (SK).
- **Visualizations**: Generates Gantt charts and dependency graphs.
- **CPM and PERT**: Supports both deterministic and probabilistic scheduling methods.

## Input Methods
1. **CSV Input**: Import task data from a CSV file.
2. **Manual Input**: Enter task details (name, duration, predecessors) directly into the tool.

## Example CSV Format

### For **CPM** and **PERT**:
```csv
Task,Predecessors,Duration
A,-,5
B,A,3
C,A,6
...
Task,Predecessors,Optimistic,MostLikely,Pessimistic
A,-,4,5,6
B,A,2,3,4
C,A,5,6,7
...
