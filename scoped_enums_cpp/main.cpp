#include <string>

using namespace ::std;


class Solider 
{
	public:
		int age;
		string name;
	struct rank_tag
	{
		rank_tag() = default;
		bool isOfficer;
		bool isNagad;
		bool habshan;
		enum evaluationRank {
			FIRED = 0,
			IDIOT,
			OK,
			GOOD,
			EVYATAR,
			MUVHAK
		} rank_eval;

	} rank;
};
int main()
{

	Solider d;
	d.age = 31;
	d.name = "aviv";
	d.rank.isOfficer = true;
	d.rank.rank_eval = Solider::rank_tag::evaluationRank::IDIOT;
	
}

