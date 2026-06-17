# question1.py

# (a) Declare node record
class Node:
    def __init__(self, data=0, nextNode=-1):
        self.data = data
        self.nextNode = nextNode


# (c)(i) Procedure to output nodes
def outputNodes(linkedList, startPointer):
    currentPointer = startPointer
    
    while currentPointer != -1:
        print(linkedList[currentPointer].data)
        currentPointer = linkedList[currentPointer].nextNode


# (d)(i) Function to add node
def addNode(linkedList, startPointer, emptyList):
    if emptyList == -1:
        return False, startPointer, emptyList

    newData = int(input("Enter data to add: "))

    # Get empty node
    newNodeIndex = emptyList
    emptyList = linkedList[emptyList].nextNode

    # Assign data
    linkedList[newNodeIndex].data = newData
    linkedList[newNodeIndex].nextNode = -1

    # If list empty
    if startPointer == -1:
        startPointer = newNodeIndex
    else:
        currentPointer = startPointer
        while linkedList[currentPointer].nextNode != -1:
            currentPointer = linkedList[currentPointer].nextNode
        
        linkedList[currentPointer].nextNode = newNodeIndex

    return True, startPointer, emptyList


# (b) Main program

# Create array
linkedList = [Node() for i in range(10)]

# Initialise data (based on given table)
linkedList[0] = Node(1, 5)
linkedList[1] = Node(5, 4)
linkedList[2] = Node(6, 7)
linkedList[3] = Node(2, 0)
linkedList[4] = Node(0, -1)
linkedList[5] = Node(0, -1)
linkedList[6] = Node(56, 0)
linkedList[7] = Node(7, -1)
linkedList[8] = Node(2, 6)
linkedList[9] = Node(8, 3)

# Pointers
startPointer = 0
emptyList = 5


# (d)(ii) Test program

print("Before adding node:")
outputNodes(linkedList, startPointer)

result, startPointer, emptyList = addNode(linkedList, startPointer, emptyList)

if result:
    print("Node added successfully")
else:
    print("No empty nodes available")

print("After adding node:")
outputNodes(linkedList, startPointer)
