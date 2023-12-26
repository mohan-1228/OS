from collections import deque
import random


def simulate_page_replacement(algorithm, page_reference, num_frames):
    """
    Simulates the specified page replacement algorithm and returns the number of page faults.

    Parameters:
    - algorithm (str): The page replacement algorithm to simulate.
    - page_reference (list): The page reference string.
    - num_frames (int): The number of frames allocated to the process.

    Returns:
    - page_faults (int): The number of page faults for the given algorithm.
    """
    if algorithm == 'FIFO':
        return fifo(page_reference, num_frames)
    elif algorithm == 'LRU':
        return lru(page_reference, num_frames)
    elif algorithm == "OPT":
        return opt(page_reference, num_frames)
    elif algorithm == "RAND":
        return rand(page_reference, num_frames)
    

def fifo(page_reference, num_frames):
    """
    Simulates the First in First out (FIFO) page replacement algorithm.

    Parameters:
    - page_reference (list): The page reference string.
    - num_frames (int): The number of frames allocated to the process.

    Returns:
    - page_faults (int): The number of page faults using FIFO algorithm.
    """
    frames = []
    page_faults = 0

    for page in page_reference:
        if page not in frames:
            if len(frames) < num_frames:
                frames.append(page)
            else:
                frames.pop(0)
                frames.append(page)
            page_faults += 1

    return page_faults

def lru(page_reference, num_frames):
    """
    Simulates the Least Recently Used (LRU) page replacement algorithm.

    Parameters:
    - page_reference (list): The page reference string.
    - num_frames (int): The number of frames allocated to the process.

    Returns:
    - page_faults (int): The number of page faults using LRU algorithm.
    """
    frames = []
    page_faults = 0

    for page in page_reference:
        if page not in frames:
            page_faults += 1
            if len(frames) == num_frames:
                frames.pop(0)
            frames.append(page)
        else:
            frames.remove(page)
            frames.append(page)

    return page_faults


def opt(page_reference, num_frames):
    """
    Simulates the Optimal (OPT) page replacement algorithm.

    Parameters:
    - page_reference (list): The page reference string.
    - num_frames (int): The number of frames allocated to the process.

    Returns:
    - page_faults (int): The number of page faults using OPT algorithm.
    """
    frames = [-1] * num_frames  # Initialize frames with -1 (indicating empty frames)
    page_faults = 0

    for i, page in enumerate(page_reference):
        if page not in frames:
            if -1 in frames:
                # If there are empty frames, put the page in an empty frame
                empty_frame = frames.index(-1)
                frames[empty_frame] = page
            else:
                # Find the page that will not be used for the longest time
                farthest_future_use = [-1] * num_frames
                for j in range(num_frames):
                    page_to_find = frames[j]
                    if page_to_find not in page_reference[i:]:
                        farthest_future_use[j] = float('inf')
                    else:
                        farthest_future_use[j] = page_reference[i:].index(page_to_find)
                
                page_to_replace = frames[farthest_future_use.index(max(farthest_future_use))]
                frames[frames.index(page_to_replace)] = page
            page_faults += 1

    return page_faults



def rand(page_reference, num_frames):
    """
    Simulates the Random (RAND) page replacement algorithm.

    Parameters:
    - page_reference (list): The page reference string.
    - num_frames (int): The number of frames allocated to the process.

    Returns:
    - page_faults (int): The number of page faults using RAND algorithm.
    """
    frames = []
    page_faults = 0

    for page in page_reference:
        if page not in frames:
            if len(frames) < num_frames:
                frames.append(page)
            else:
                # Replace a random page
                idx = random.randint(0, num_frames - 1)
                frames[idx] = page
            page_faults += 1

    return page_faults





# Read input from standard input

page_reference = list(map(int, input().split()))
num_frames = int(input())
page_algorithms = [input().strip() for _ in range(4)]

# Echo the page reference string
print("\nPage Reference String:")
for i in range(0, len(page_reference), 20):
    print(" ".join(str(page) for page in page_reference[i:i+20]))

# Echo the number of frames allocated
print("Number of Frames:", num_frames)

# Simulate and output the results for each page replacement algorithm
for algorithm in page_algorithms:
    page_faults = simulate_page_replacement(algorithm, page_reference, num_frames)
    print(f"{algorithm}: {page_faults}")