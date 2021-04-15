#include <stdio.h>
#include <time.h>
#include <iostream>
#include <map>
#include "RemoteServer.h"
#include <random>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include "../LRUCache/LRUCache.h"

RemoteServer::RemoteServer(int propagationTime) {
    this->propagationTime = propagationTime;
    this->currSize = 0;
    this->fileMap = map<int, File*>();
}

RemoteServer::RemoteServer(int propagationTime, int size, double paretoShape, double paretoScale) {
    this->propagationTime = propagationTime;
    this->currSize = size;
    this->fileMap = map<int, File*>();
    this->paretoScale = paretoScale;
    this->paretoShape = paretoShape;
    this->populateServer();
}

int RemoteServer::getFile(int fileId) {
    if (this->fileMap.find(fileId) == this->fileMap.end()) {
        return -1;
    }
    int currValue = fileMap[fileId]->value;
    return currValue;
}

void RemoteServer::insertFile(int fileId, int value) {
    if (this->fileMap.find(fileId) != this->fileMap.end()) {
        this->fileMap[fileId]->value = value;
        return;
    }

    File *file = new File(fileId, value);
    this->fileMap[fileId] = file;
    this->currSize++;
}

void RemoteServer::populateServer() {

    const gsl_rng_type * T;
    gsl_rng_env_setup();
    T = gsl_rng_default;
    const gsl_rng * r = gsl_rng_alloc (T);

    for (int i = 1; i < this->currSize + 1; i++){
        double randomSample = gsl_ran_pareto(r, this->paretoShape, this->paretoScale);
        File *file = new File(i, int(randomSample));
        this->fileMap[i] = file;
    }
}

RemoteServer::~RemoteServer() {
    map<int, File*>::iterator iter;
    for (iter = this->fileMap.begin(); iter != this->fileMap.end(); iter++)  {
        delete iter->second;
    }
}