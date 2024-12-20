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

def plot_graphs(description_file_path):
    graphs = parse_description_file(description_file_path)
    overlay_active = False

    for graph in graphs:
        x, y = load_graph_data(graph["file_path"])

        if not graph["overlay"] and overlay_active:
            plt.legend()
            plt.show()
            overlay_active = False

        if graph["plot_type"] == "plot":
            plt.plot(x, y, label=graph["title"])
        elif graph["plot_type"] == "stem":
            plt.stem(x, y, label=graph["title"], use_line_collection=True)
        else:
            raise ValueError(f"Unsupported plot type: {graph['plot_type']}")

        plt.title(graph["title"])
        plt.xlabel(graph["x_label"])
        plt.ylabel(graph["y_label"])

        if graph["overlay"]:
            overlay_active = True
        else:
            plt.legend()
            plt.show()

    if overlay_active:
        plt.legend()
        plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Plot graphs based on a description file.")
    parser.add_argument("description_file", type=str, help="Path to the description file.")
    args = parser.parse_args()

    if not os.path.exists(args.description_file):
        raise FileNotFoundError(f"Description file not found: {args.description_file}")

    plot_graphs(args.description_file)
