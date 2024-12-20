import glob
import numpy as np
import matplotlib.pyplot as plt
from pprint import pprint

# my 
from classes.DataParser import DataParser 

# visuals 
from graph import graph_noise, graph_buffer, graph_show, graph_channel_noise, graph_channel_ref, graph_buffer_dem, graph_buffer_dem_ref, graph_autocorr_ref, graph_autocorr

simInfo = {} 

with DataParser("./data/simdata.txt", "\n") as dataParse:
    simData = dataParse.all_data
    if simData: 
        simData = simData[0] 

        simInfo['txFreq'] = int(simData[0])
        simInfo['bufferSize'] = int(simData[1])
        simInfo['oversampleMult'] = int(simData[2])
        simInfo['sigma'] = float(simData[3])
        simInfo['txPower'] = float(simData[4])
        simInfo['noisePower'] = float(simData[5]) 

        pprint(simInfo)
    else: 
        print("[simData] No data found")


# graph Buffer
with DataParser("./data/buffer.txt", separator="") as dataParse: 
    simBuffer = dataParse.array_to_np()
    if simBuffer: 
        graph_buffer(simBuffer)
    else:
        print("[simBuffer] No data found")


# graph Noise (all)
with DataParser("./data/noise_*.txt", "\n") as dataParse: 
    simNoise = dataParse.array_to_np()
    if simNoise: 
        graph_noise(simNoise)
    else:
        print("[simNoise] No data found")

# graph BufferX
with DataParser("./data/buffer_*.txt", separator="") as dataParse: 
    simBufferX = dataParse.array_to_np()
    if simBufferX: 
        graph_buffer(simBufferX)
    else:
        print("[simBufferX] No data found")

# graph ChannelNoise
with DataParser("./data/ch_nouse_*.txt", separator="\n") as dataParse: 
    simChannelNoise = dataParse.array_to_np()
    if simChannelNoise: 
        graph_channel_noise(simChannelNoise)
    else:
        print("[simChannelNoise] No data found")

# graph ChannelRef
with DataParser("./data/ch_reference_*.txt", separator="\n") as dataParse: 
    simChannelRef = dataParse.array_to_np()
    if simChannelRef: 
        graph_channel_ref(simChannelRef)
    else:
        print("[simChannelRef] No data found")

# graph BufferDemRef
with DataParser("./data/buffer_dem_ref.txt", separator="") as dataParse: 
    simBufferDemRef = dataParse.array_to_np()
    if simBufferDemRef: 
        graph_buffer_dem_ref(simBufferDemRef)
    else:
        print("[simBufferDemRef] No data found")

# graph AutoCorrRef
with DataParser("./data/ch_autocorr.txt", separator="\n") as dataParse: 
    simAutoCorrRef = dataParse.array_to_np()
    if simAutoCorrRef: 
        graph_autocorr_ref(simAutoCorrRef)
    else:
        print("[simAutoCorrRef] No data found")

# graph AutoCorr
with DataParser("./data/ch_autocorr_*.txt", separator="\n") as dataParse: 
    simAutoCorr = dataParse.array_to_np()
    if simAutoCorr: 
        graph_autocorr(simAutoCorr)
    else:
        print("[simAutoCorr] No data found")

# graph BufferDem
with DataParser("./data/buffer_dem_*.txt", separator="") as dataParse: 
    simBufferDem = dataParse.array_to_np()
    if simBufferDem: 
        graph_buffer_dem(simBufferDem)
    else:
        print("[simBufferDem] No data found")

graph_show()