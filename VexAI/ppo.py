import numpy as np
from tensorflow.keras import backend as K



#stolen from https://github.com/LuEE-C/PPO-Keras/blob/master/Main.py
def proximal_policy_optimization_loss(advantage, old_prediction, LOSS_CLIPPING, ENTROPY_LOSS):
    def loss(y_true, y_pred):
        prob = K.sum(y_true * y_pred, axis=-1)
        old_prob = K.sum(y_true * old_prediction, axis=-1)
        r = prob/(old_prob + 1e-10)
        return -K.mean(K.minimum(r * advantage, K.clip(r, min_value=1 - LOSS_CLIPPING, max_value=1 + LOSS_CLIPPING) * advantage) + ENTROPY_LOSS * -(prob * K.log(prob + 1e-10)))
    return loss