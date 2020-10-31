#ifndef _TOURNAMENT_SELECTOR_H_
#define _TOURNAMENT_SELECTOR_H_
#include "Selector.h"

class TournamentSelector : public Selector
{
public:
    TournamentSelector(int numPlayers);
    virtual tuple<shared_ptr<Creature>, shared_ptr<Creature>> Select(
        const list<shared_ptr<Creature>> &population) override;

protected:
    int m_numPlayers = -1;
    shared_ptr<Creature> DoTournament(const list<shared_ptr<Creature>> &population);
};

#endif