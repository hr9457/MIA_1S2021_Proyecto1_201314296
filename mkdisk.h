#ifndef MKDISK_H
#define MKDISK_H


class mkdisk
{
private:
    int size;
public:
    mkdisk();
    void setSize(int);
    int getSize();
    void creacionDisco();
    void mensaje();
};

#endif // MKDISK_H
