import numpy as np
np.set_printoptions(threshold=np.inf)
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.models import load_model
import pandas as pd

def classifier_pridictor_keras(photo_path, txtfilename):


    test_path = photo_path
    # print(test_path)
    test_datagen = ImageDataGenerator(rescale=1. / 255)
    test_batches = test_datagen.flow_from_directory(test_path, target_size=(50, 50),
                                                    classes=['dc', 'nm'],
                                                    batch_size=64, save_format='jpeg', shuffle=False)


    model = load_model('D:/Desktop back up/新建文件夹 (3)/chromCNN/0906ranseti_fullmodel$8X.h5')

    model.compile(optimizer=Adam(lr=0.001, beta_1=0.9, beta_2=0.999, epsilon=None, decay=1e-6, amsgrad=False),
                  loss='categorical_crossentropy', metrics=['accuracy'], loss_weights=None, sample_weight_mode=None,
                  weighted_metrics=None, target_tensors=None)


    predictions = model.predict_generator(test_batches, steps=len(test_batches), max_queue_size=10, workers=1,
                                          use_multiprocessing=False, verbose=0)
    # print(test_batches.filenames)
    pd.set_option('max_rows', 800)
    m1 = np.argmax(np.round(predictions), axis=1)
    # print(m1)

    str = ','.join(test_batches.filenames)
    # print(str)

    str2 = ",".join('%s' % id for id in m1)
    # print(str2)



    full_name = str + ',' + str2

    print(txtfilename)
    fname = r"E:/tmp/321"
    fname1 = r"\t" + txtfilename + ".txt"
    # fname1 = txtfilename + ".txt"
    fname += fname1
    # t = open(fname.decode('utf8').encode('gbk'),'w')
    t = open(fname, 'w')
    full_name = photo_path + '\\dc\\,' + str + ',' + str2
    t.write(full_name)
    t.close()

    return full_name

