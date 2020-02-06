/*
** EPITECH PROJECT, 2019
** JukeBox
** File description:
** Globpp.cpp
*/

#include <glob.h>
#include "Utility/Globpp.hpp"
#include "Error.hpp"

vector<string> globpp(const string &pattern)
{
    vector<string> pathList;
    glob_t pglob;

    if (glob(pattern.c_str(), GLOB_MARK, NULL, &pglob))
        throw ERROR("glob error");
    for (size_t i = 0; i < pglob.gl_pathc; i++)
        pathList.push_back(string(pglob.gl_pathv[i]));
    globfree(&pglob);
    return pathList;
}
