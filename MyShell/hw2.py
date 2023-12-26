'''Name: Mohan Thapa 
    CSC535 Data Mining 
    10 october 2023'''




import pandas as pd
import numpy as np
import math


file = pd.read_csv("input.csv")
def entropy(labels):
    elements, count = np.unique(labels, return_counts = True)
    probability = count / len(labels)
    entropy = -np.sum(probability * np.log2(probability))
    return entropy

def information_gain(data, attribute, target_attribute):
    total_entropy = entropy(data[target_attribute])
    weighted_entropy = 0
    values = data[attribute].unique()
    for value in values:
        subset = data[data[attribute] == value]
        weighted_entropy += len(subset) / len(data) * entropy(subset[target_attribute])
    return total_entropy - weighted_entropy

attributes = file.columns[:-1]  
target_attribute = "Hire"

best_attribute = None
max_info_gain = -1

for attribute in attributes:
    info_gain = information_gain(file, attribute, target_attribute)
    if info_gain > max_info_gain:
        max_info_gain = info_gain
        best_attribute = attribute

print(f"Best test attribute is  {best_attribute}")
print(f"Information gained by splitting on Level is  {max_info_gain:.3f}")