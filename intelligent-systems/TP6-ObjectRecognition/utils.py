from torch.utils.data import TensorDataset
from torchvision.transforms import ToPILImage
import matplotlib.pyplot as plt
import matplotlib.patches as patches
import torch
import torch.nn as nn


def normalize_data(data:TensorDataset):
    """
    Normalize the data
    """
    features = data.tensors[0]
    bboxes = data.tensors[1]
    labels = data.tensors[2]
    
    mean = [0.485, 0.456, 0.406]
    std = [0.229, 0.224, 0.225]
    
    new_features = (features - torch.tensor(mean).view(1,3,1,1)) / torch.tensor(std).view(1,3,1,1)

    return TensorDataset(new_features, bboxes, labels)


classes = ['person' , 'bird', 'cat', 'cow',
           'dog', 'horse', 'sheep', 'aeroplane',
           'bicycle', 'boat', 'bus', 'car',
           'motorbike', 'train', 'bottle', 'chair',
           'diningtable', 'pottedplant', 'sofa', 'tvmonitor']

index_to_class = {i:classes[i] for i in range(len(classes))}

def visualize_image(image, bbox, label):
    """
    Visualize the image with the bounding box.
    """
    to_pil = ToPILImage()
    image = to_pil(image)
    width, height = image.size
    bbox = bbox.detach().numpy()
    label = label.item()
    cx, cy, w, h = bbox[0], bbox[1], bbox[2], bbox[3]
    xmin = (cx - w/2) * width
    xmax = (cx + w/2) * width
    ymin = (cy - h/2) * height
    ymax = (cy + h/2) * height
    
    fig, ax = plt.subplots(1)
    ax.imshow(image)
    rect = patches.Rectangle((xmin, ymin), xmax - xmin, ymax - ymin, linewidth=3, edgecolor='r',    facecolor='none')
    ax.text(xmin, ymin-5, index_to_class[label], fontsize=12, color='red')
    ax.add_patch(rect)
    plt.show()
    

    

def train_loop(model, trainloader, validationloader,optimizer, device,
               writer,
               epochs=10):
    """
    Train the model
    """
    losses = []
    val_losses = []
    class_loss = nn.CrossEntropyLoss() # Loss function for classification
    bbox_loss = nn.SmoothL1Loss() # Loss function for bounding box regression
    model.to(device) # Move model to device
    #TODO # Set the model to training mode
    for epoch in range(epochs):
        running_loss = 0
        correct = 0
        for i, data in enumerate(trainloader):
            inputs, bboxes, labels = #TODO # Get the inputs, bounding boxes and labels
            inputs, bboxes, labels = #TODO # Move the inputs, bounding boxes and labels to device
            outputs = #TODO #Verify the model output and get the class and bounding box predictions
            
            _, preds = #TODO #Get the class prediction
            loss_label =  #TODO #Calculate the loss for the class prediction
            loss_box = #TODO #Calculate the loss for the bounding box prediction
            batch_loss = loss_label + loss_box  #Calculate the total loss
        
            # Backpropagation and optimization step using batch_loss
            #TODO 
            #TODO 
            #TODO 
            
            running_loss += batch_loss.item()
        
            correct += torch.sum(preds==labels.data).detach().cpu().numpy()
        epoch_loss = #TODO #Calculate the epoch loss
        epoch_acc = #TODO #Calculate the epoch accuracy
        losses.append(epoch_loss)
        
        val_epoch_loss, val_epoch_acc = validation_loop(model, validationloader, device)
        
        writer.add_scalars('Loss', {'train':epoch_loss, 'validation':val_epoch_loss}, epoch)
        writer.add_scalars('Accuracy', {'train':epoch_acc, 'validation':val_epoch_acc}, epoch)
        print(f'Epoch {epoch+1}/{epochs} || Train Loss: {epoch_loss:.4f} || Train Acc: {epoch_acc:.4f} || Val Loss: {val_epoch_loss:.4f} || Val Acc: {val_epoch_acc:.4f}')
    
    return losses, val_epoch_losses, epoch_acc, val_epoch_acc

def validation_loop(model, loader, device):
    """
    Validation loop
    """
    #TODO # Set the model to evaluation mode
    class_loss = nn.CrossEntropyLoss()
    bbox_loss = nn.SmoothL1Loss()
    with torch.no_grad():
        running_loss = 0.0
        correct = 0
        for i, data in enumerate(loader):
            inputs, bboxes, labels = data
            inputs, bboxes, labels = inputs.to(device), bboxes.to(device), labels.to(device)
            outputs = #TODO #Get the model output
            _, preds = #TODO #Get the class prediction
            loss_label = #TODO #Calculate the loss for the class prediction
            loss_box = #TODO #Calculate the loss for the bounding box prediction
            batch_loss = loss_label + loss_box
            running_loss += batch_loss.item()
            
            correct += torch.sum(preds==labels.data).detach().cpu().numpy()
        epoch_loss = #TODO #Calculate the epoch loss
        epoch_acc = #TODO #Calculate the epoch accuracy
    return epoch_loss, epoch_acc

def predict(model, data, show=True):
    """
    Predict the data
    """
    feature = data.unsqueeze(0)
    class_pred, bbox_pred = model(feature)
    class_pred = torch.argmax(class_pred, dim=1)
    bbox_pred = bbox_pred.squeeze().detach()
    
    # Re-normalize image
    mean = torch.tensor([0.485, 0.456, 0.406]).view(3, 1, 1)
    std = torch.tensor([0.229, 0.224, 0.225]).view(3, 1, 1)
    data = data * std + mean 
    if show:
        visualize_image(data, bbox_pred, class_pred)
    return class_pred, bbox_pred