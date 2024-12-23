import matplotlib.pyplot as plt
import numpy as np
import argparse


# Parse command-line arguments
parser = argparse.ArgumentParser(description="Plot Zipfian Log(Rank) vs Log(Frequency).")
parser.add_argument("file", type=str, help="Path to the file containing Zipfian output data.")
args = parser.parse_args()

# Load the data from the file
file_path = args.file

# Skip the header and load log values
log_ranks = []
log_freqs = []

with open(file_path, "r") as file:
    next(file)  # Skip header line
    for line in file:
        columns = line.split()
        log_rank = float(columns[3])
        log_freq = float(columns[4])
        log_ranks.append(log_rank)
        log_freqs.append(log_freq)

# Plot the data
plt.figure(figsize=(10, 6))
plt.scatter(log_ranks, log_freqs, color="b", label="Log-Log Data")
plt.plot(
    log_ranks,
    np.poly1d(np.polyfit(log_ranks, log_freqs, 1))(log_ranks),
    color="r",
    linestyle="--",
    label="Best-Fit Line",
)

plt.xlabel("Log(Rank)")
plt.ylabel("Log(Frequency)")
plt.title("Zipfian Distribution Test: Log(Rank) vs Log(Frequency)")
plt.legend()
plt.grid()
plt.show()
