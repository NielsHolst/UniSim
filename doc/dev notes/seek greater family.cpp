template <class T> T peekOneGreaterFamily(QString name, QObject *object);
template <class T> T seekOneGreaterFamily(QString name, QObject *object);
template <class T> QList<T> seekGreaterFamily(QString name, QObject *object);


//! Finds one greater family for object or none
/*!
    Finds one greater family matching name and type, or null
*/
template <class T> T peekOneGreaterFamily(QString name, QObject *object) {
    QList<T> greaterFamily = seekGreaterFamily<T>(name, object);
    if (greaterFamily.size() == 0)
        return 0;
    if (greaterFamily.size() > 1)
        throw Exception("More than one greater family called '" + name +"'" +
                        " of class " + typeid(T).name(), object);
    return greaterFamily.at(0);

}

//! Finds one greater family for object or throws an exception
/*!
    Finds one greater family matching name and type, or throws an Exception
*/
template <class T> T seekOneGreaterFamily(QString name, QObject *object) {
    T greaterFamily = peekOneGreaterFamily<T>(name, object);
    if (!greaterFamily)
        throw Exception("No greater family called '" + name + " of class " + typeid(T).name(), object);
    return greaterFamily;

}

//! Finds all greater family for object
/*!
    Finds all greater family matching name and type
*/
template <class T> QList<T> seekGreaterFamily(QString name, QObject *object) {
    QList<T> all = seekMany<T>(name);
    QList<T> closeFamily = seekDescendants<T>(name, object);
    closeFamily.append(this);
    QList<T> result;
    for (int i = 0; i < all.size(); ++i) {
        bool isCloseFamily = closeFamily.contains(all[i]);
        if (!isCloseFamily)
            result.append(all[i]);
    }
    return result;
}

