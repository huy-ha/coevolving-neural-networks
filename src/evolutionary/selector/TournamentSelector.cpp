#include "TournamentSelector.h"
#include <EvolutionaryUtils.h>

TournamentSelector::TournamentSelector(int numPlayers) : Selector()
{
    m_numPlayers = numPlayers;
}

shared_ptr<Creature> TournamentSelector::DoTournament(const list<shared_ptr<Creature>> &population)
{
    int populationCount = int(population.size());
    vector<shared_ptr<Creature>> players;
    if (m_numPlayers == population.size())
    {
        return *max_element(population.begin(), population.end(), Creature::PtrCompare);
    }
    else if (m_numPlayers > population.size())
    {
        cout << "ERROR:PARENTS LESS THAN TOURNAMENT SIZE" << endl;
        throw exception();
    }
    while (players.size() < m_numPlayers)
    {
        auto it = population.begin();
        int idx = SafeRandomInt(0, populationCount - 1);
        advance(it, idx);

        bool notunique = any_of(players.begin(), players.end(), [&](auto p) {
            return p->Id() == (*it)->Id();
        });
        if (!notunique)
        {
            players.push_back(*it);
        }
    }
    sort(players.begin(), players.end(), Creature::PtrCompare);
    return players[0];
}

tuple<shared_ptr<Creature>, shared_ptr<Creature>> TournamentSelector::Select(
    const list<shared_ptr<Creature>> &population)
{
    if (population.size() == 2)
    {
        return make_tuple(population.front(), population.back());
    }
    else if (population.size() < 2)
    {
        cout << "ERROR: POPULATION TOO SMALL" << endl;
        throw exception();
    }
    shared_ptr<Creature> p2, p1 = DoTournament(population);
    do
    {
        p2 = DoTournament(population);
    } while (p2->Id() == p1->Id());
    return make_tuple(p1, p2);
}