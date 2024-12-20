import matplotlib.pyplot as plt
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
    return x, y

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
        fig, ax = plt.subplots(len(graphs), 1, figsize=(8, 4 * len(graphs)))
        if len(graphs) == 1:
            ax = [ax] 

        for idx, graph in enumerate(graphs):
            current_ax = ax[idx]
            x, y = load_graph_data(graph["file_path"])

            if graph["plot_type"] == "plot":
                current_ax.plot(x, y, label=graph["title"])
            elif graph["plot_type"] == "stem":
                current_ax.stem(x, y, label=graph["title"], use_line_collection=True)
            else:
                raise ValueError(f"Unsupported plot type: {graph['plot_type']}")

            current_ax.set_title(graph["title"])
            current_ax.set_xlabel(graph["x_label"])
            current_ax.set_ylabel(graph["y_label"])
            current_ax.legend()

        plt.tight_layout()
        plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Plot graphs based on a description file.")
    parser.add_argument("description_file", type=str, help="Path to the description file.")
    args = parser.parse_args()

    if not os.path.exists(args.description_file):
        raise FileNotFoundError(f"Description file not found: {args.description_file}")

    plot_graphs(args.description_file)
