namespace essentialTools
{
	template<class T>
	struct point
	{
		T x, y;

		point(T _x, T _y)
		{
			x = _x;
			y = _y;
		}
	};
}