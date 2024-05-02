1. lastInsertionDateTime = getLatestInsertionDateForRobot
2. timeDifference = checkTimesDiffrence(lastInsertionDateTime)
3. if (timeDifference > 1) {
        sendMissingData();
    } else {
        proceed();
    }
4. *after Initial Procedure*
5. sendDataRegulary(){
    getCurrentReads
    checkDifference (1-10)
    getDiffrentSensorsData: []
    if diff >= 1 {
        sendNewData(temp1 = 1, ...)
    }
}