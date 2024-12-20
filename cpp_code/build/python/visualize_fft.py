# import matplotlib.pyplot as plt
# import numpy as np
# import argparse
# import os

# def parse_description_file(description_file_path):
#     graphs = []
#     with open(description_file_path, 'r') as file:
#         for line in file:
#             line = line.strip()
#             if not line:
#                 continue
#             parts = line.split(';')
#             if len(parts) < 7:
#                 raise ValueError(f"Invalid format in description file: {line}")
#             graph = {
#                 "file_path": parts[0].strip(),
#                 "plot_type": parts[1].strip(),
#                 "columns": parts[2].strip(),
#                 "title": parts[3].strip().strip('"'),
#                 "x_label": parts[4].strip().strip('"'),
#                 "y_label": parts[5].strip().strip('"'),
#                 "overlay": parts[6].strip().lower() == 'true'
#             }
#             graphs.append(graph)
#     return graphs

# def load_graph_data(file_path):
#     x, y = [], []
#     with open(file_path, 'r') as file:
#         for line in file:
#             line = line.strip()
#             if not line:
#                 continue
#             parts = line.split(';')
#             if len(parts) < 2:
#                 raise ValueError(f"Invalid data format in file: {file_path}")
#             x.append(float(parts[1]))
#             y.append(float(parts[0]))
#     return np.array(x), np.array(y)

# def parse_subplot_info(file_name):
#     if not file_name.startswith('(') or ')' not in file_name:
#         raise ValueError(f"Invalid file name format: {file_name}")
#     parts = file_name.split(')')[0].strip('(').split(',')
#     if len(parts) != 3:
#         raise ValueError(f"Invalid subplot info in file name: {file_name}")
#     return int(parts[0]), int(parts[1]), int(parts[2])

# def plot_graphs(description_file_path):
#     graphs = parse_description_file(description_file_path)

#     subplots = {}
#     for graph in graphs:
#         file_name = os.path.basename(graph["file_path"])
#         subplot_index, _, _ = parse_subplot_info(file_name)
#         if subplot_index not in subplots:
#             subplots[subplot_index] = []
#         subplots[subplot_index].append(graph)

#     for subplot_index, graphs in sorted(subplots.items()):
#         # Create a figure with subplots for each graph and its spectrum
#         fig, ax = plt.subplots(len(graphs), 2, figsize=(12, 4 * len(graphs)))

#         for idx, graph in enumerate(graphs):
#             x, y = load_graph_data(graph["file_path"])

#             # Plot the main graph
#             if graph["plot_type"] == "plot":
#                 ax[idx, 0].plot(x, y, label=graph["title"])
#             elif graph["plot_type"] == "stem":
#                 ax[idx, 0].stem(x, y, label=graph["title"], use_line_collection=True)
#             else:
#                 raise ValueError(f"Unsupported plot type: {graph['plot_type']}")

#             ax[idx, 0].set_title(graph["title"])
#             ax[idx, 0].set_xlabel(graph["x_label"])
#             ax[idx, 0].set_ylabel(graph["y_label"])
#             ax[idx, 0].legend()

#             N = len(y)
#             Y = np.fft.fft(y)
#             freq = np.fft.fftfreq(N, d=(x[1] - x[0])) 

#             ax[idx, 1].plot(freq[:N // 2], np.abs(Y)[:N // 2], label='Spectrum', color='orange')
#             ax[idx, 1].set_title(f'Spectrum of {graph["title"]}')
#             ax[idx, 1].set_xlabel('Freq')
#             ax[idx, 1].set_ylabel('Magnitude')
#             ax[idx, 1].legend()

#         plt.tight_layout()
#         plt.show()

# if __name__ == "__main__":
#     parser = argparse.ArgumentParser(description="Plot graphs based on a description file.")
#     parser.add_argument("description_file", type=str, help="Path to the description file.")
#     args = parser.parse_args()

#     if not os.path.exists(args.description_file):
#         raise FileNotFoundError(f"Description file not found: {args.description_file}")

#     plot_graphs(args.description_file)


import matplotlib.pyplot as plt
import numpy as np
import argparse
import os

def parse_description_file(description_file_path):
    graphs = []
    with open(description_file_path, 'r') as file:
        for line in file:
            line = line.strip()
            if not line:
                continue
            parts = line.split(';')
            if len(parts) < 7:
                raise ValueError(f"Invalid format in description file: {line}")
            graph = {
                "file_path": parts[0].strip(),
                "plot_type": parts[1].strip(),
                "columns": parts[2].strip(),
                "title": parts[3].strip().strip('"'),
                "x_label": parts[4].strip().strip('"'),
                "y_label": parts[5].strip().strip('"'),
                "overlay": parts[6].strip().lower() == 'true'
            }
            graphs.append(graph)
    return graphs

def load_graph_data(file_path):
    x, y = [], []
    with open(file_path, 'r') as file:
        for line in file:
            line = line.strip()
            if not line:
                continue
            parts = line.split(';')
            if len(parts) < 2:
                raise ValueError(f"Invalid data format in file: {file_path}")
            x.append(float(parts[1]))
            y.append(float(parts[0]))
    return np.array(x), np.array(y)

def parse_subplot_info(file_name):
    if not file_name.startswith('(') or ')' not in file_name:
        raise ValueError(f"Invalid file name format: {file_name}")
    parts = file_name.split(')')[0].strip('(').split(',')
    if len(parts) != 3:
        raise ValueError(f"Invalid subplot info in file name: {file_name}")
    return int(parts[0]), int(parts[1]), int(parts[2])

def plot_graphs(description_file_path):
    graphs = parse_description_file(description_file_path)

    subplots = {}
    for graph in graphs:
        file_name = os.path.basename(graph["file_path"])
        subplot_index, _, _ = parse_subplot_info(file_name)
        if subplot_index not in subplots:
            subplots[subplot_index] = []
        subplots[subplot_index].append(graph)

    for subplot_index, graphs in sorted(subplots.items()):
        fig, ax = plt.subplots(len(graphs), 2, figsize=(12, 4 * len(graphs)))

        spectra = []
        colors = plt.cm.viridis(np.linspace(0, 1, len(graphs))) 

        for idx, graph in enumerate(graphs):
            x, y = load_graph_data(graph["file_path"])

            if graph["plot_type"] == "plot":
                ax[idx, 0].plot(x, y, label=graph["title"])
            elif graph["plot_type"] == "stem":
                ax[idx, 0].stem(x, y, label=graph["title"], use_line_collection=True)
            else:
                raise ValueError(f"Unsupported plot type: {graph['plot_type']}")

            ax[idx, 0].set_title(graph["title"])
            ax[idx, 0].set_xlabel(graph["x_label"])
            ax[idx, 0].set_ylabel(graph["y_label"])
            ax[idx, 0].legend()

            N = len(y)
            Y = np.fft.fft(y)
            freq = np.fft.fftfreq(N, d=(x[1] - x[0]))

            spectra.append((freq[:N // 2], np.abs(Y)[:N // 2]))

        ax_spectrum = fig.add_subplot(1, 2, 2)
        for idx, (freq, magnitude) in enumerate(spectra):
            ax_spectrum.plot(freq, magnitude, label=graphs[idx]["title"], color=colors[idx], alpha=0.7)

        ax_spectrum.set_title('Spectra')
        ax_spectrum.set_xlabel('Freq')
        ax_spectrum.set_ylabel('Magnitude')
        ax_spectrum.set_ylim(0, 150)
        ax_spectrum.legend()

        plt.tight_layout()
        plt.show()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Plot graphs based on a description file.")
    parser.add_argument("description_file", type=str, help="Path to the description file.")
    args = parser.parse_args()

    if not os.path.exists(args.description_file):
        raise FileNotFoundError(f"Description file not found: {args.description_file}")

    plot_graphs(args.description_file)
