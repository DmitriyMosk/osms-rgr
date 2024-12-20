import matplotlib.pyplot as plt 

def graph_buffer(plot_data): 
    bit_values = [int(bit) for bit in plot_data[0]]
    
    # Генерируем индексы для оси X
    x_values = list(range(len(bit_values)))
    
    # Создаем график
    plt.figure(figsize=(12, 6))
    plt.step(x_values, bit_values, where='mid')
    plt.ylim(-0.5, 1.5)
    plt.xlabel('Bit Index')
    plt.ylabel('Bit Value')
    plt.title('Buffer state')
    plt.grid(True)


def graph_noise(plot_data):
    for i, data in enumerate(plot_data):
        fig = plt.figure(figsize=(10, 5))
        plt.plot(data, label=f"Dataset {i+1}")
        plt.legend()
        plt.title(f"Processed Noise Data - Dataset {i+1}")

def graph_channel_ref(plot_data):
    for i, data in enumerate(plot_data):
        fig = plt.figure(figsize=(10, 5))
        plt.plot(data, label=f"Dataset {i+1}")
        plt.legend()
        plt.title(f"Симуляция передачи данных без шума {i+1}")

def graph_channel_noise(plot_data):
    for i, data in enumerate(plot_data):
        fig = plt.figure(figsize=(10, 5))
        plt.plot(data, label=f"Dataset {i+1}")
        plt.legend()
        plt.title(f"Симуляция передачи данных в условиях шума {i+1}")

def graph_buffer_dem_ref(plot_data): 
    bit_values = [int(bit) for bit in plot_data[0]]
    
    # Генерируем индексы для оси X
    x_values = list(range(len(bit_values)))
    
    # Создаем график
    plt.figure(figsize=(12, 6))
    plt.step(x_values, bit_values, where='mid')
    plt.ylim(-0.5, 1.5)
    plt.xlabel('Bit Index')
    plt.ylabel('Bit Value')
    plt.title('Buffer IN Reference state')
    plt.grid(True)

def graph_buffer_dem(plot_data): 
    for i, data in enumerate(plot_data):
        bit_values = [int(bit) for bit in data]
        
        # Генерируем индексы для оси X
        x_values = list(range(len(bit_values)))
        
        # Создаем график
        fig = plt.figure(figsize=(10, 5))
        plt.step(x_values, bit_values, where='mid', label=f"Dataset {i+1}")
        plt.ylim(-0.5, 1.5)
        plt.xlabel('Bit Index')
        plt.ylabel('Bit Value')
        plt.title('Buffer IN Reference state')
        plt.grid(True)  
        plt.legend()

def graph_autocorr_ref(plot_data): 
    fig = plt.figure(figsize=(10, 5))
    plt.plot(plot_data[0])
    plt.title(f"Кореляция идеального сигнала")

def graph_autocorr(plot_data): 
    for i, data in enumerate(plot_data):
        fig = plt.figure(figsize=(10, 5))
        plt.plot(data, label=f"Dataset {i+1}")
        plt.legend()
        plt.title(f"Функция автокорреляции {i+1}")

def graph_show(): 
    plt.show()