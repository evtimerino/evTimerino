#include "common.h"
#include <exposure.h>

Exposure::Exposure(Buzzer& b) : buzzer(b)  {
    newAdj = new node();
    newAdj->timeCounter = 0;
    newAdj->value = 0;
    newAdj->area = 0;
    newAdj->precision = 3;
    newAdj->precisionIdx = 1;
    newAdj->steps = 0;
    newAdj->type = Adjustment::NONE;
}

Exposure::~Exposure() {
    delete newAdj;
}

void Exposure::resetNewAdj() {
    newAdj->timeCounter = 0;
    newAdj->value = 0;
    newAdj->area = 0;
    newAdj->precision = 3;
    newAdj->precisionIdx = 1;
    newAdj->steps = 0;
    newAdj->type = Adjustment::NONE;
}

void Exposure::clear() {
    node *newItr = head;
    while (newItr != nullptr)
    {
        head = head->next;
        delete newItr;
        newItr = head;
    }
    head = nullptr;
    tail = head;
    itr = head;
    dodgeItr = head;
    dodgeTimeCounters = 0;
    size = 0;
    position = 0;
    dodgeSize = 0;
    burnSize = 0;
    switch (precisionMultiplier)
    {
    case 2:
        baseTimeCounter = 40;
        break;
    case 3:
        baseTimeCounter = 80;
        break;
    case 4:
        baseTimeCounter = 160;
        break;
    case 5:
        baseTimeCounter = 320;
        break;
    case 6:
        baseTimeCounter = 640;
        break;
    default:
        break;
    }
    baseTimeCounterAdjusted = baseTimeCounter;
    precisionIdx = 2;
    newPrecisionIdx = precisionIdx;
    precision = precisions[precisionIdx];
    steps = precision * precisionMultiplier;
    resetNewAdj();
}

void Exposure::restart() {
    if (mode == Mode::EXPOSURE) {
        baseTime = true;
        if (head != nullptr) {
            itr = head;
            position = 0;
        }
    }
    if (mode == Mode::LINEAR) {
        getLinearTimeCounter();
    }
}

void Exposure::setAdjustmentUp() {
    uint8_t adjPrecision = isNewAdjustment() ? newAdj->precision : itr->precision;
    uint8_t unit = genUnit(adjPrecision);
    if (itr != nullptr) {
        if (itr->type == Adjustment::BURN) {
            buzzer.doubleBuzz();
            float newPower = (float)(steps + ((unit * itr->value) + unit)) / (float)precision;
            uint16_t newTime = lrint(10*pow(2.0f,newPower));
            if (newTime <= 5120) {
                itr->timeCounter = newTime - baseTimeCounter;
                itr->value++;
            }
            return;
        }
        if (itr->type == Adjustment::DODGE) {
            float newPower = (float)(steps - ((unit * itr->value) + unit)) / (float)precision;
            uint16_t newTime = lrint(10*pow(2.0f,newPower));
            if (itr->value > 1) {
                float prevPower = (float)(steps - (unit * itr->value)) / (float)precision;
                int16_t prevTime = lrint(10*pow(2.0f,prevPower));
                baseTimeCounterAdjusted = baseTimeCounterAdjusted + ((baseTimeCounter - prevTime) - (baseTimeCounter - newTime));
                itr->timeCounter = baseTimeCounter - newTime;
                itr->value--;
            }
            return;
        }
    } else {
        if (newAdj->type == Adjustment::NONE) {
            newAdj->type = Adjustment::BURN;
        }
        if (newAdj->type == Adjustment::BURN || newAdj->type == Adjustment::NONE) {
            float newPower = (float)(steps + ((unit * newAdj->value) + unit)) / (float)precision;
            uint16_t newTime = lrint(10*pow(2.0f,newPower));
            if (newTime <= 5120) {
                newAdj->timeCounter = newTime - baseTimeCounter;
                newAdj->value++;
            }
        }
        if (newAdj->type == Adjustment::DODGE) {
            float newPower = (float)(steps - ((unit * newAdj->value) + unit)) / (float)precision;
            uint16_t newTime = lrint(10*pow(2.0f,newPower));
            newAdj->timeCounter = baseTimeCounter - newTime;
            newAdj->value--;
            if (newAdj->value == 0) {
                newAdj->timeCounter = 0;
                newAdj->type = Adjustment::NONE;
            }
        }
    }
}

void Exposure::setAdjustmentDown() {
    uint8_t adjPrecision = isNewAdjustment() ? newAdj->precision : itr->precision;
    uint8_t unit = genUnit(adjPrecision);
    if (itr != nullptr) {
        if (itr->type == Adjustment::DODGE) {
            float newPower = (float)(steps - ((unit * itr->value) + unit)) / (float)precision;
            int16_t newTime = lrint(10*pow(2.0f,newPower));
            if ((baseTimeCounterAdjusted - newTime)  > 10) {
                float prevPower = (float)(steps - (unit * itr->value)) / (float)precision;
                int16_t prevTime = lrint(10*pow(2.0f,prevPower));
                baseTimeCounterAdjusted = baseTimeCounterAdjusted - ((baseTimeCounter - newTime) - (baseTimeCounter - prevTime));
                itr->timeCounter = baseTimeCounter - newTime;
                itr->value++;
            }
        }

        if (itr->type == Adjustment::BURN) {
            float newPower = (float)(steps + ((unit * itr->value) - unit)) / (float)precision;
            uint16_t newTime = lrint(10*pow(2.0f,newPower));
            if (itr->value >1 ) {
                itr->timeCounter = newTime - baseTimeCounter;
                itr->value--;
            }
        }
    } else {
        if (newAdj->type == Adjustment::NONE) {
            newAdj->type = Adjustment::DODGE;
        }
        if (newAdj->type == Adjustment::DODGE || newAdj->type == Adjustment::NONE) {
            float newPower = (float)(steps - ((unit * newAdj->value) + unit)) / (float)precision;
            int16_t newTime = lrint(10*pow(2.0f,newPower));
            if (baseTimeCounterAdjusted -(baseTimeCounter - newTime) > 10) {
                if (newAdj->type == Adjustment::NONE) newAdj->type = Adjustment::DODGE;
                newAdj->timeCounter = baseTimeCounter - newTime;
                newAdj->value++;
            }
        }
        if (newAdj->type == Adjustment::BURN) {
            float newPower = (float)(steps + ((unit * newAdj->value) - unit)) / (float)precision;
            uint16_t newTime = lrint(10*pow(2.0f,newPower));
            newAdj->timeCounter = newTime - baseTimeCounter;
            newAdj->value--;
            if (newAdj->value == 0) {
                newAdj->timeCounter = 0;
                newAdj->type = Adjustment::NONE;
            }
        }
    }
}

void Exposure::saveAdjustment() {
    if (size == MAX_ADJUSTMENTS) {
        buzzer.tripleBuzz();
        return;
    }
    if (newAdj->type == Adjustment::NONE) return;
    node* adj = new node();
    adj->timeCounter = newAdj->timeCounter;
    adj->value = newAdj->value;
    adj->type = newAdj->type;
    adj->precision = newAdj->precision;
    adj->precisionIdx = newAdj->precisionIdx;
    adj->steps = newAdj->steps;
    adj->next = nullptr;

    if (head == nullptr) {
        head = adj;
        tail = head;
        dodgeItr = head;
        adj->area = 0;
        size++;
        if (newAdj->type == Adjustment::DODGE ) {
            baseTimeCounterAdjusted -= adj->timeCounter;
            dodgeSize++;
        } else {
            burnSize++;
        }
        itr = nullptr;
        resetNewAdj();
        return;
    }

    if (adj->type == Adjustment::BURN) {
        adj->area = tail->area + 1;
        tail->next = adj;
        tail = tail->next;
        adj = adj->next;
        burnSize++;
        size++;
        itr = nullptr;
        resetNewAdj();
        return;
    }
    
    if (adj->type == Adjustment::DODGE) {
        if (head->type != Adjustment::DODGE) {
            adj->next = head;
            head = adj;
            baseTimeCounterAdjusted -= adj->timeCounter;
            dodgeItr = head;
            while (adj != nullptr)
            {
                adj = adj->next;
            }
            
        } else {
            if (dodgeItr->next == nullptr) {
                adj->area = tail->area + 1;
                baseTimeCounterAdjusted -= adj->timeCounter;
                tail->next = adj;
                tail = tail->next;
                dodgeItr = tail;
                adj = adj->next;
            } else {
                adj->next = dodgeItr->next;
                baseTimeCounterAdjusted -= adj->timeCounter;
                dodgeItr->next = adj;
                dodgeItr->next->area = dodgeItr->area + 1;
                dodgeItr = dodgeItr ->next;
                while (adj != nullptr)
                {
                    adj = adj->next;
                }
            }
        }
        dodgeSize++;
        size++;
        itr = nullptr;
        resetNewAdj();
        return;
    }
}

uint8_t Exposure::getValue() {
    if (itr->type == Adjustment::BURN) return (itr->value) * -1;
    return itr->value;
}

uint8_t Exposure::getArea() {
    return itr->area;
}


uint8_t Exposure::getDodgeSize() {
    return dodgeSize;
}

uint8_t Exposure::getBurnSize() {
    return burnSize;
}

uint8_t Exposure::getPrecision() {
    return precision;
}

uint8_t Exposure::getPrecisionIdx() {
    return precisionIdx;
}

uint16_t Exposure::getTimeCounter() {
    switch (mode)
    {
    case Mode::EXPOSURE:
        if (baseTime) { 
            return baseTimeCounterAdjusted;
        }
        if (itr != nullptr) {
            uint16_t adjTimeCounter = itr->timeCounter;
            itr = itr->next;
            return adjTimeCounter;
        }
        break;
    case Mode::TESTSTRIP:
        return getTestStripTimeCounter();
        break;
    case Mode::LINEAR:
        return getLinearTimeCounter();
        break;
    case Mode::ADJUSTMENT:
        return newAdj->timeCounter;
        break;
    default:
        break;
    }
    return 0;
}

bool Exposure::isNewAdjustment() {
    if (itr == nullptr) {
        return true;
    }
    return false;
}

bool Exposure::isNewAdjustmentBurn() {
    if (newAdj->type == Adjustment::BURN) return true;
    return false;
}

void Exposure::resetIndex() {
    itr = head;
    resetNewAdj();
}

void Exposure::nextAdjustment() {
    if (head != nullptr && itr == head) {
        newAdj->timeCounter = 0;
        newAdj->value = 0;
    }
    if (itr == nullptr) {
        itr = head;
        return;
    }
    if (itr != nullptr) itr = itr->next;
}

uint16_t Exposure::getNewAdjustmentTimeCounter() {
    return newAdj->timeCounter;
}

uint8_t Exposure::getNewAdjustmentValue() {
    return newAdj->value;
}

uint16_t Exposure::getAdjustmentTimeCounter() {
    if (itr == nullptr && newAdj == nullptr) {
        return 0;
    } else if (itr == nullptr && newAdj != nullptr) {
        return newAdj->timeCounter; 
    } else {
        return itr->timeCounter;
    }
}

uint8_t Exposure::getAdjustmentValue() {
    if (itr == nullptr && newAdj == nullptr) {
        return 0;
    } else if (itr == nullptr && newAdj != nullptr) {
        return newAdj->value; 
    } else {
        return itr->value;
    }
}

Adjustment Exposure::getAdjustmentType() {
    return itr->type;
}

Adjustment Exposure::getNewAdjustmentType() {
    return newAdj->type;
}

uint8_t Exposure::getAdjustmentArea() {
    return itr->area;
}

void Exposure::next(){
    if (itr != nullptr) {
        baseTime = false;
        return;
    }
    if (itr == nullptr) {
        baseTime = true;
        itr = head;
        return;
    }
}

bool Exposure::lastAdj() {
    if (itr == nullptr) {
        return true;
    }
    if (itr->next == nullptr) {
        return true;
    }
    return false;
}

void Exposure::setBaseExposureUp() {
    steps++;
    float power = (float)steps / (float)precision;
    baseTimeCounter = lrint(10*pow(2.0f,power));
    updateAdjustments();
}

void Exposure::setBaseExposureDown() {
    steps--;
    float power = (float)steps / (float)precision;
    baseTimeCounter = lrint(10*pow(2.0f,power));
    updateAdjustments();
}

void Exposure::updateAdjustments() {
    node *newItr = head;
    float newPower;
    int16_t newTime;
    baseTimeCounterAdjusted = baseTimeCounter;
    while (newItr != nullptr)
    {
        if (newItr->precision == 0) {
            newItr = newItr->next;
            continue;
        }

        uint8_t unit = genUnit(newItr->precision);

        switch (newItr->type)
        {
        case Adjustment::DODGE:
            newPower = (float)(steps - (unit * newItr->value)) / (float)precision;
            newTime = lrint(10*pow(2.0f,newPower));
            newItr->timeCounter = baseTimeCounter - newTime;
            baseTimeCounterAdjusted -= newItr->timeCounter;
            break;
        case Adjustment::BURN:
            newPower = (float)(steps + (unit * newItr->value)) / (float)precision;
            newTime = lrint(10*pow(2.0f,newPower));
            newItr->timeCounter = newTime - baseTimeCounter;;
            break;
        default:
            break;
        }
        newItr = newItr->next;
    }
}

bool Exposure::getBaseTime() {
    if (mode == Mode::TESTSTRIP) return false;
    return baseTime;
}

void Exposure::setBaseTime() {
    baseTime = false;
}

uint16_t Exposure::getBaseTimeCounter() {
    return baseTimeCounterAdjusted;
}

void Exposure::remove() {
    if (head == nullptr) {
        return;
    }
    if (itr == head && itr->next == nullptr) {
        if (itr-> type == Adjustment::DODGE) {
            dodgeSize = 0;
            dodgeTimeCounters = 0;
            dodgeItr = head;
            baseTimeCounterAdjusted += itr->timeCounter;
        } else {
            burnSize = 0;
        }
        size = 0;
        position = 0;
        node * newNode = itr;
        head = nullptr;
        itr = head;
        delete(newNode);
        return;
    } 
    node *newItr1 = head;
    while (newItr1 != nullptr)
    {
        if (newItr1->next == itr) {
            break;
        }
        newItr1 = newItr1->next;
    }
    if (itr->next == nullptr) {
        if (itr->type == Adjustment::DODGE) {
            dodgeSize--;
            baseTimeCounterAdjusted += itr->timeCounter;
            dodgeItr = newItr1;
            size--;
            position--;
        } else {
            burnSize--;
            size--;
            position--;
        }
        node *newNode = itr;
        itr = newItr1;
        delete(newNode);
    } else {
        node *newItr2 = itr->next;
        if (itr->type == Adjustment::DODGE) {
            while (newItr2 != nullptr && newItr2->type == Adjustment::DODGE)
            {
                newItr2->area--;
                newItr2 = newItr2->next;
            }
            dodgeSize--;
            baseTimeCounterAdjusted += itr->timeCounter;
            dodgeItr = newItr1;
        } else {
            while (newItr2 != nullptr && newItr2->type == Adjustment::BURN)
            {
                newItr2->area--;
                newItr2 = newItr2->next;
            }
            burnSize--;
        }
        node *newNode = itr;
        newItr1->next = itr->next;
        itr = newItr1;
        position--;
        size--;
        position--;
        delete(newNode);
    }
}

void Exposure::setPrecision(uint8_t idx) {
    precision = precisions[idx];
    precisionIdx = idx;
    steps = precision * precisionMultiplier;
}

void Exposure::testStripNext() {
    if ((teststripMode == Teststrip::SEPARATE_A || teststripMode == Teststrip::INCREMENTAL_A) && teststripSteps == 3) {
        teststripSteps = -3;
        return;
    }
    teststripSteps++;
}

uint16_t Exposure::getTestStripTimeCounter() {
    if (teststripMode == Teststrip::SEPARATE_B || teststripMode == Teststrip::SEPARATE_A) {
        float power = ((float)tsSteps + teststripSteps) / (float)tsPrecision;
        return lrint(10*pow(2.0f,power));
    }
    if (teststripMode == Teststrip::SPLIT_GRADE){
        if (teststripSteps%2 == 0) return baseTimeCounter;
        uint8_t adjusted_steps = 1;
        if (teststripSteps > 1) adjusted_steps = teststripSteps-1;
        float power = ((float)tsSteps + adjusted_steps) / (float)tsPrecision;
        uint16_t newTime = lrint(10*pow(2.0f,power));
        return newTime - baseTimeCounter;
    }
    if (teststripMode == Teststrip::INCREMENTAL_B) {
        if (teststripSteps == 0) return baseTimeCounter;
        float power = ((float)tsSteps + (teststripSteps)) / (float)tsPrecision;
        uint16_t newTime = lrint(10*pow(2.0f,power));
        if (teststripSteps >= 2) {
            power = ((float)tsSteps + (teststripSteps-1)) / (float)tsPrecision;
            uint16_t prevTime = lrint(10*pow(2.0f,power));
            return (newTime - baseTimeCounter) - (prevTime - baseTimeCounter);
        }
        return newTime - baseTimeCounter;
    }
    if (teststripMode == Teststrip::INCREMENTAL_A) {
        if (teststripSteps == -3) {
            float power = ((float)tsSteps + (teststripSteps)) / (float)tsPrecision;
            return lrint(10*pow(2.0f,power));
        }
        if (teststripSteps >= -2) {
            float power = ((float)tsSteps - 3) / (float)tsPrecision;
            uint16_t firtTime = lrint(10*pow(2.0f,power));
            
            power = ((float)tsSteps + (teststripSteps)) / (float)tsPrecision;
            uint16_t newTime = lrint(10*pow(2.0f,power));
            if (teststripSteps > -2) {
                power = ((float)tsSteps + ((teststripSteps-1))) / (float)tsPrecision;
                uint16_t prevTime = lrint(10*pow(2.0f,power));
                return (newTime - firtTime) - (prevTime - firtTime);
            }
            return newTime - firtTime;
        }
    }
    return 0;
}

void Exposure::resetTestStripSteps() {
    if (teststripMode == Teststrip::SEPARATE_A || teststripMode == Teststrip::INCREMENTAL_A) {
        teststripSteps = -3;
        return;
    }
    teststripSteps = 0;
}

int8_t Exposure::getTestStripSteps() {
    if (teststripMode == Teststrip::SEPARATE_A || teststripMode == Teststrip::INCREMENTAL_A) {
        return teststripSteps;
    }
    switch (teststripMode)
    {
    case Teststrip::SPLIT_GRADE:
        return (teststripSteps / 2) + 1;
    case Teststrip::INCREMENTAL_B:
        return teststripSteps;
    default:
        return teststripSteps + 1;
    }
}

void Exposure::setMode(Mode m) {
    mode = m;
}

void Exposure::setTestStripMode(Teststrip m) {
    teststripMode = m;
}

void Exposure::switchBaseTime() {
    if (baseTime) {
        baseTime = false;
    } else {
        baseTime = true;
    }
}

void Exposure::resetBaseTime() {
    baseTime = true;
}

Teststrip Exposure::getTestStripMode() {
    return teststripMode;
}

Mode Exposure::getMode() {
    return mode;
}

uint8_t Exposure::getSize() {
    return size;
}

void Exposure::setPrecisionMultiplier(uint8_t s) {
    precisionMultiplier = s;
}

void Exposure::setStartTime(uint8_t st, bool runtime) {
    precisionMultiplier = st;
    clear();
    steps = precision * precisionMultiplier;
}

uint8_t Exposure::getPrecisionMultiplier() {
    return precisionMultiplier;
}

void Exposure::switchTestStripMode() {
    if ((teststripMode == Teststrip::INCREMENTAL_A || teststripMode == Teststrip::SEPARATE_A) && teststripSteps != -3) {
        return;
    }
    if ((teststripMode == Teststrip::INCREMENTAL_B || teststripMode == Teststrip::SEPARATE_B || teststripMode == Teststrip::SPLIT_GRADE) && teststripSteps != 0) {
        return;
    }
    switch (teststripMode)
    {
    case Teststrip::SEPARATE_A:
        teststripMode = Teststrip::INCREMENTAL_A;
        teststripSteps = -3;
        break;
    case Teststrip::INCREMENTAL_A:
        teststripMode = Teststrip::SPLIT_GRADE;
        teststripSteps = 0;
        break;
    case Teststrip::SPLIT_GRADE:
        teststripMode = Teststrip::SEPARATE_B;
        teststripSteps = 0;
        break;        
    case Teststrip::SEPARATE_B:
        teststripMode = Teststrip::INCREMENTAL_B;
        teststripSteps = 0;
        break;
    case Teststrip::INCREMENTAL_B:
        teststripMode = Teststrip::SEPARATE_A;
        teststripSteps = -3;
        break;
    default:
        break;
    }
}

void Exposure::setPrecisionUp() {
    if (newPrecisionIdx > 3) {
        newPrecisionIdx = 3;
    }
    if (newPrecisionIdx < 3) {
        newPrecisionIdx++;
    }
}

void Exposure::setPrecisionDown() {
    if (newPrecisionIdx > 3) {
        newPrecisionIdx = 3;
    }
    if (newPrecisionIdx > 0) {
        newPrecisionIdx--;
    }
}

uint8_t Exposure::getNewPrecision() {
    return precisions[newPrecisionIdx];
}

void Exposure::updatePrecision() {
    if (newPrecisionIdx != precisionIdx) {
        uint8_t selectedPrecisionIdx = newPrecisionIdx;
        clear();
        setPrecision(selectedPrecisionIdx);
        newPrecisionIdx = selectedPrecisionIdx;
    }
    splitState = precisionIdx < 5;
}

bool Exposure::splitSteps() {
    if (!splitState || precisionIdx >= 5) {
        splitState = false;
        buzzer.tripleBuzz();
        return false;
    }

    uint8_t prevPrecision = precision;
    precisionIdx = precisionIdx + 1;
    newPrecisionIdx = precisionIdx;
    precision = precisions[precisionIdx];
    steps = lrint(((float)steps * (float)precision) / (float)prevPrecision) + 1;
    float power = ((float)steps) / (float)precision;
    baseTimeCounter = lrint(10*pow(2.0f,power));
    baseTimeCounterAdjusted = baseTimeCounter;
    if (head != nullptr) {
        updateAdjustments();
    }

    if (precisionIdx >= 5) {
        splitState = false;
    }

    return true;
}

void Exposure::setLinearUp() {
    if (linearPrecision == LinearPrecision::TENTHS && linearBaseTimeCounter < 9998) {
        linearBaseTimeCounter += 1;
    }

    if (linearPrecision == LinearPrecision::SECONDS && linearBaseTimeCounter < 9989) {
        linearBaseTimeCounter += 10;
    }
}

void Exposure::setLinearDown() {
    if (linearPrecision == LinearPrecision::TENTHS && linearBaseTimeCounter > 1) {
        linearBaseTimeCounter -= 1;
    }

    if (linearPrecision == LinearPrecision::SECONDS && linearBaseTimeCounter > 10) {
        linearBaseTimeCounter -= 10;
    }
}

void Exposure::switchLinearPrecision() {
    if (linearPrecision == LinearPrecision::SECONDS) {
        linearPrecision = LinearPrecision::TENTHS;
    } else {
        linearPrecision = LinearPrecision::SECONDS;
    }
}

void Exposure::resetLinearTimeCounter() {
    linearBaseTimeCounter = 100;
}

uint16_t Exposure::getLinearTimeCounter() {
    return linearBaseTimeCounter;
}

LinearPrecision Exposure::getLinearPrecision() {
    return linearPrecision;
}

void Exposure::setTestStripPrecisionUp() {
    if (tsPrecisionIdx == 5) {
        return;
    }
    tsPrecisionIdx++;
    if (tsPrecisionIdx > precisionIdx) {
        tsPrecision = precisions[tsPrecisionIdx];
        tsSteps = (tsSteps / precisions[tsPrecisionIdx-1]) * tsPrecision;
    }
    if (tsPrecisionIdx == precisionIdx) {
        tsPrecision = precisions[tsPrecisionIdx];
        tsSteps = steps;
    }
    if (tsPrecisionIdx < precisionIdx) {
        tsPrecision = precisions[tsPrecisionIdx];
        tsSteps = steps / (precision / tsPrecision);
    }
}

void Exposure::setTestStripPrecisionDown() {
    if (precision == 3) {
        return;
    }
    if (tsPrecisionIdx == 0) {
        return;
    }
    tsPrecisionIdx--;
    if (tsPrecisionIdx == precisionIdx) {
        tsSteps = steps;
        tsPrecision = precisions[tsPrecisionIdx];
    }
    if (tsPrecisionIdx < precisionIdx) {
        tsPrecision = precisions[tsPrecisionIdx];
        tsSteps = steps / (precision / tsPrecision);
    } 
    if (tsPrecisionIdx > precisionIdx) {
        tsPrecision = precisions[tsPrecisionIdx];
        tsSteps = (tsSteps / precisions[tsPrecisionIdx+1]) * tsPrecision;
    }
}

uint8_t Exposure::getTestStripPrecision() {
    return tsPrecision;
}

void Exposure::resetTestStrip() {
    tsPrecisionIdx = precisionIdx;
    tsPrecision = precision;
    tsSteps = steps;
}

void Exposure::switchAdjPrecision() {
    if (!isNewAdjustment()) {
        return;
    }

    newAdj->precisionIdx++;
    if (newAdj->precisionIdx > 2) {
        newAdj->precisionIdx = 0;
    }

    newAdj->precision = precisions[newAdj->precisionIdx];
    newAdj->timeCounter = 0;
    newAdj->value = 0;
    newAdj->area = 0;
    newAdj->type = Adjustment::NONE;
}

uint8_t Exposure::getAdjPrecision() {
    if (isNewAdjustment()) {
        return newAdj->precision;
    }
    return itr->precision;
}

uint8_t Exposure::genUnit(uint8_t adjPrecision) {

    return precision / adjPrecision;
}