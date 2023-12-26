import csv
import numpy as np

# Function to calculate Euclidean distance between two data points
def euclidean_distance(point1, point2):
    return np.sqrt(np.sum((point1 - point2) ** 2))

# Function to predict the class label using KNN with weighted voting
def knn_with_weighted_voting(train_data, test_sample, k):
    distances = []  # To store (distance, class) pairs for all training samples

    for train_sample in train_data:
        # Calculate Euclidean distance between the test sample and each training sample
        distance = euclidean_distance(train_sample[1:], test_sample[1:])
        distances.append((distance, train_sample[0]))  # (distance, class) pair

    # Sort distances in ascending order and select the top K neighbors
    k_nearest_neighbors = sorted(distances)[:k]

    # Initialize counters for each class
    class_votes = {0: 0, 1: 0, 2: 0, 3: 0, 4: 0, 5: 0, 6: 0, 7: 0, 8: 0, 9: 0}

    # Weighted voting: Inverse distance is used for voting
    for distance, class_label in k_nearest_neighbors:
        if distance == 0:
            class_votes[class_label] += 1
        else:
            class_votes[class_label] += 1 / distance

    # Predict the class with the maximum weighted vote
    predicted_class = max(class_votes, key=class_votes.get)
    return predicted_class

# Load MNIST training and test data (skip header lines)
train_data = np.loadtxt("MNIST_train.csv", delimiter=",", skiprows=1)
test_data = np.loadtxt("MNIST_test.csv", delimiter=",", skiprows=1)

# Choose the value of K (you may use cross-validation or other methods)
k = 3

# Initialize counters for accuracy and misclassified samples
correct_predictions = 0
total_samples = len(test_data)

# Predict the class label for each test sample and calculate accuracy
for test_sample in test_data:
    desired_class = int(test_sample[0])
    computed_class = knn_with_weighted_voting(train_data, test_sample, k)
    
    if desired_class == computed_class:
        correct_predictions += 1

# Calculate accuracy and the number of misclassified test samples
accuracy = (correct_predictions / total_samples) * 100
misclassified_samples = total_samples - correct_predictions

# Print the results
print("Value of K:", k)
print("Accuracy Rate:", accuracy, "%")
print("Misclassified Test Samples:", misclassified_samples)
print("Total Number of Test Samples:", total_samples)
