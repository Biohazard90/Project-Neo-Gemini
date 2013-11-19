#ifndef CONFIG_H
#define CONFIG_H

class Globals;

class Config
{
public:

    static void LoadConfig(Globals *globals);
    static void SaveConfig(Globals *globals);

};

#endif // CONFIG_H
