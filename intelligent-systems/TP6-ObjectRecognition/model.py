import torch.nn as nn
import torch
from torchvision.models.resnet import resnet18
from torchinfo import summary

    
class ResnetObjectDetector(nn.Module):
    """ Resnet18 based feature extraction layers """
    def __init__(self, nb_classes):
        super().__init__()
        # copy resnet up to the last conv layer prior to fc layers, and flatten
        features = list(resnet18(weights='DEFAULT').children())[:9]
        self.features = nn.Sequential(*features, nn.Flatten())

        # freeze all ResNet18 layers during the training process
        for param in self.features.parameters():
            param.requires_grad = False

        # create classifier path for class label prediction
        self.classifier = nn.Sequential(
            # First Block
            #TODO
            #TODO
            #TODO
            # Second Block
            #TODO
            #TODO
            #TODO
            
            # Final Fully connected Layer
            #TODO
        )

        # create regressor path for bounding box coordinates prediction
        self.bbox_regressor = nn.Sequential(
            # First Block
            #TODO
            #TODO
            
            # Second Block
            #TODO
            #TODO
            
            # Final Block
            #TODO  #4 coordinates for the bounding box
            #TODO 
        )

    def forward(self, x):
        x = self.features(x)
        class_pred = self.classifier(x)
        bbox_pred = self.bbox_regressor(x)
        return class_pred, bbox_pred
