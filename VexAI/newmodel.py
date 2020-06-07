import gym
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.optimizers import Adam
from collections import deque
import random
from Envrioment import *
import numpy as np
#deifne deep neural net calls
class DQN:
    def __init__(self, env):
        self.env = env
        #define memeonry
        self.memory = deque(maxlen=2000)
        #future rewards depertiaon
        self.gamma = 0.95
        #expoloration/explotation intial value, decay and minium
        self.epsilon = 1.0
        self.epsilon_decay = 0.995
        self.epsilon_min = 0.1
        #learing rate
        self.learing_rate = 0.01
        #target model smoothing rate
        self.tau = .125
        #define model
        self.model = self.create_model()
        #define target model
        self.target_model = self.create_model()


    
    def create_model(self):
        #define sequintail model
        model = Sequential()
        #get state shape
        #add input layer
        model.add(Dense(48, input_shape=(81,), 
            activation='relu'))
        #add layers
        model.add(Dense(48, activation="relu"))
        model.add(Dense(48, activation="relu"))
        #add output layer
        model.add(Dense(19))
        #comple model with adam opismiser and meen square rror lost
        model.compile(loss='mean_squared_error', 
            optimizer=Adam(lr=self.learing_rate))
        #return model
        return model

        #tell model to remember state
    def remember(self, state, action, reward, newstate, done):
        self.memory.append([state, action, reward, newstate, done])

    #replay and train on a sample of memery data commtied to memory
    def replay(self):
        batch_size = 32
        #if not enough info return
        if len(self.memory) < batch_size:
            return
        #get random sample of states with bath size
        samples = random.sample(self.memory, batch_size) 
        #itterate over samples
        for sample in samples:
            state, action, reward, newstate, done = sample
            #predict action
            target = self.target_model.predict(state)
            #jsut aplly pervious reward if state was finshing state
            if done:
                target[0][action] = reward
            #else use q function to weight both current reward and futre reward
            #cacluate futre reward baseupon strengh of model predictions
            else:
                #get futere aciton cofinede for reward
                Q_future = max(self.target_model.predict(newstate)[0])
                #add the reward for the action pluse futre
                target[0][action] = reward + Q_future * self.gamma
                #fit model to reward offets function returns 
                print('fitting model')
                self.model.fit(state, target, epochs=1, verbose=0)
    #train target
    def train_target(self):
        weights = self.model.get_weights()
        target_weights = self.target_model.get_weights()
        for i in range(len(target_weights)):
            #slowy convert target weights over time to the priamy model weights by tau each time. ie it will be 75 og weight. and 25 new weight and the next tik mroeso
            target_weights[i] = weights[i] * self.tau + target_weights[i] * (1 - self.tau)
        self.target_model.set_weights(target_weights)

    #act model
    def act(self, state):
        self.epsilon *= self.epsilon_decay
        self.epsilon = max(self.epsilon_min, self.epsilon)
        if np.random.random() < self.epsilon:
            #return random action space value
            return random.randint(0, 18)
        #else pic max vlaue
        return np.argmax(self.model.predict(state))

        #save model
    def save_model(self, fn):
        self.model.save(fn)

#define main function
def main():
    env = Match(200)

    trials = 1000

    trial_len = 500

    dqn_agent = DQN(env=env)
    steps = []
    #runn through trials
    for trial in range(trials):
        cur_state = env.reset().reshape(1, 81)
        for step in range(trial_len):
            #get action
            action = dqn_agent.act(cur_state)
            #step env
            newstate, reward, done = env.step(0, action)
            print('reward: {}'.format(reward))
            #reshape newstate
            newstate = newstate.reshape(1, 81)
            #reward = reward if not done else -20
            #remember date
            dqn_agent.remember(cur_state, action, reward, newstate, done)
            #replay sates from memeory to train
            dqn_agent.replay()
            #train target
            dqn_agent.train_target()

            #set curretns tate to new state
            cur_state = newstate
            #break if done
            if done:
                break
        #if cant compleate tiral in centan number of steps then fail teh robot 
        if step >= 199:
            print("Failed to complete in trial {}".format(trial))
            #save very tent model
            if step % 10 == 0:
                dqn_agent.save_model("trial-{}.model".format(trial))
        else:
            print("Completed in {} trials".format(trial))
            #save winning model
            dqn_agent.save_model("success.model")
            break
main()
