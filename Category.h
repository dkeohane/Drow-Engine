#ifndef CATEGORY_H_
#define CATEGORY_H_

enum Categories
{
	None = 0,
	UncounterableLv1            = 1ul <<  0,
	Projectile                  = 1ul <<  1,
	Unblockable                 = 1ul <<  2,
	Player            = 1ul <<  3
};

inline Categories operator|(Categories& lhs, Categories& rhs)
{
	return static_cast<Categories>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline Categories operator&(Categories& lhs, Categories& rhs)
{
	return static_cast<Categories>(static_cast<int>(lhs) & static_cast<int>(rhs));
}

inline Categories operator~(Categories& lhs)
{
	return static_cast<Categories>(~static_cast<int>(lhs));
}

#endif