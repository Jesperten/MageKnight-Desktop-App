#ifndef CITY_H
#define CITY_H

#include <QString>

class City
{
public:
    City();

    // Public Members
    QString mName = "";
    QString mColor = "";
    QString mCityOwner = "";
    int mId = 0;
    int mLevel = 0;
    int mMonstersRemaining = 0;
    int mMonsters = 0;
    bool mConquered = false;
    bool mDiscovered = false;
};

#endif // CITY_H
