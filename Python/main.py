# Press Shift+F10 to execute it or replace it with your code.
# Press Double Shift to search everywhere for classes, files, tool windows, actions, and settings.
import realsenseconfig as rs_config  # Initalize - Parameter
import cv2
import  numpy as np
from Objection import Objection,Rcet
model_weight = "./dataset/yolov3-320.weights"
model_cfg = "./dataset/yolov3-320.cfg"
model_classname="./dataset/coco.names"
def Dectection(net,image):
    color=image
    h, w = image.shape[:2]
    blobImage = cv2.dnn.blobFromImage(image, 1.0 / 255.0, (224, 224), None, True, False);
    outNames = net.getUnconnectedOutLayersNames()
    net.setInput(blobImage)
    outs = net.forward(outNames)
    # Put efficiency information.
    t, _ = net.getPerfProfile()
    fps = 1000 / (t * 1000.0 / cv2.getTickFrequency())
    label = 'FPS: %.2f' % fps
    cv2.putText(image, label, (0, 15), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0))
    # prepare data for boxes
    classIds = []
    confidences = []
    boxes = []
    for out in outs:
        for detection in out:
            scores = detection[5:]
            classId = np.argmax(scores)
            confidence = scores[classId]
            # numbers are [center_x, center_y, width, height]
            if confidence > 0.5:
                center_x = int(detection[0] * w)
                center_y = int(detection[1] * h)
                width = int(detection[2] * w)
                height = int(detection[3] * h)
                left = int(center_x - width / 2)
                top = int(center_y - height / 2)
                classIds.append(classId)
                confidences.append(float(confidence))
                boxes.append([left, top, width, height])
    indices = cv2.dnn.NMSBoxes(boxes, confidences, 0.5, 0.4)
    for i in indices:
        i = i[0]
        box = boxes[i]
        left = box[0]
        top = box[1]
        width = box[2]
        height = box[3]
        cv2.rectangle(image, (left, top), (left + width, top + height), (0, 0, 255), 2, 8, 0)
        cv2.putText(image, classes[classIds[i]], (left, top),cv2.FONT_HERSHEY_SIMPLEX, 1.0, (255, 255, 0), 2)
        Objection_vec.append(Objection(Rcet(left,left+width,top,top+height),classes[classIds[i]]))
    return image


if __name__ == '__main__':
    # print(rs_config.D435_para.color_to_depth_translation)
    rs_config.pipeline.start(rs_config.config)
    cv2.namedWindow('RealSenseRGB', cv2.WINDOW_AUTOSIZE)
    cv2.namedWindow('RealSenseDepth', cv2.WINDOW_AUTOSIZE)
    cv2.namedWindow('RealSenseDepthBrighter', cv2.WINDOW_AUTOSIZE)
    with open(model_classname, 'rt') as f:
        classes = f.read().rstrip('\n').split('\n')
    # load ObjectionDectection model
    net = cv2.dnn.readNetFromDarknet(model_cfg, model_weight)
    # set back-end
    net.setPreferableBackend(cv2.dnn.DNN_BACKEND_OPENCV)#DNN_BACKEND_INFERENCE_ENGINE on Openvino
    net.setPreferableTarget(cv2.dnn.DNN_TARGET_CPU)
    while True:
        Objection_vec=[]
        rs_config.D435_para.refresh_mat()
        # print(rs_config.D435_para.depthmat)
        Obj_img=Dectection(net,rs_config.D435_para.colormat) #Gather and paint information
        for Obj in Objection_vec:
            # print(Obj.classname, ":", Obj.PCL)
            Position='(%d %d %d)' %(Obj.PCL[0],Obj.PCL[1],Obj.PCL[2])
            # print(Obj.Point[1])
            cv2.putText(Obj_img, Position, (int(Obj.Point[1]), int(Obj.Point[0])), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0))
        cv2.imshow('RealSenseRGB',Obj_img)
        cv2.imshow('RealSenseDepth',rs_config.D435_para.depthmat)

        #--Make depthmat-image brighter
        img = rs_config.D435_para.depthmat*5
        cv2.imshow('RealSenseDepthBrighter', img)

        Keyvalue = cv2.waitKey(1)
        if Keyvalue==27:
            cv2.destoryAllWindows()
            break