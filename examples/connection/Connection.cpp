#include "Connection.h"

Connection::SESSION_T Connection::connect(wpp::Security& security) {
    return NULL; //TODO: Implemnt
}

void Connection::disconnect(SESSION_T session) {
    //TODO: Implemnt
}

bool Connection::sessionCmp(SESSION_T session1, SESSION_T session2) {
    return true; //TODO: Implemnt
}

bool Connection::sendPacket(Packet packet) {
    return true; //TODO: Implemnt
}