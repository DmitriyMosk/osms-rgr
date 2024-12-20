import numpy as np
import matplotlib.pyplot as plt

def calculate_ber(original_bits, decoded_bits):
    """
    Вычисляет Bit Error Rate (BER).
    """
    errors = np.sum(original_bits != decoded_bits)
    return errors / len(original_bits)

def encode_bits_multilevel(bits, bit_level=2):
    """
    Кодирует биты для указанных уровней (2, 4, 8 и т.д.).
    """
    levels = 2 ** bit_level
    step = 1 / (levels - 1)
    encoded = []

    for i in range(0, len(bits), bit_level):
        chunk = bits[i:i + bit_level]
        value = int("".join(map(str, chunk)), 2)
        encoded.append(value * step)

    return np.array(encoded)

def decode_values_multilevel(values, bit_level=2):
    """
    Декодирует значения обратно в биты для заданного уровня.
    """
    levels = 2 ** bit_level
    step = 1 / (levels - 1)
    decoded = []

    for v in values:
        index = min(max(int(round(v / step)), 0), levels - 1)
        bits = list(map(int, f'{index:0{bit_level}b}'))
        decoded.extend(bits)

    return np.array(decoded)

def simulate_ber_multilevel(sigma_values, oversampling_rates, bit_level=2, num_bits=10000):
    """
    Симулирует BER для разных уровней шума и передискретизации для многобитных уровней.
    """
    original_bits = np.random.randint(0, 2, num_bits)
    encoded = encode_bits_multilevel(original_bits, bit_level=bit_level)

    ber_results = {}
    for oversampling in oversampling_rates:
        ber_results[oversampling] = []
        for sigma in sigma_values:
            # Oversampling
            oversampled = np.repeat(encoded, oversampling)
            
            # Add noise
            noise = np.random.normal(0, sigma, len(oversampled))
            noisy_signal = oversampled + noise

            # Decoding
            averaged_signal = noisy_signal.reshape(-1, oversampling).mean(axis=1)
            decoded_bits = decode_values_multilevel(averaged_signal, bit_level=bit_level)

            # Calculate BER
            ber = calculate_ber(original_bits[:len(decoded_bits)], decoded_bits)
            ber_results[oversampling].append(ber)

    return ber_results

# Параметры моделирования
sigma_values = np.arange(0, 3.05, 0.25)
oversampling_rates = [10, 20, 50]
bit_levels = [2, 3, 4]  # Для 2, 4, 8 уровней кодирования (2^n)

# Моделирование и построение графиков
for bit_level in bit_levels:
    ber_results = simulate_ber_multilevel(sigma_values, oversampling_rates, bit_level=bit_level)

    # Построение графика
    plt.figure(figsize=(10, 6))
    for oversampling, ber in ber_results.items():
        plt.plot(sigma_values, ber)
    
    plt.title(f'Bit Error Rate for {2**bit_level}-level encoding')
    plt.xlabel('Sigma')
    plt.ylabel('Bit Error Rate')
    plt.ylim(0, 1)  # Устанавливаем ось Y от 0 до 1
    plt.grid()
    plt.show()

