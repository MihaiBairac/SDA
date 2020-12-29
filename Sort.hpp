#ifndef SORT_HPP
#define SORT_HPP

#include <cstddef> // size_t
#include <cassert>
#include "Utility.hpp"
#include "Vector.hpp"

namespace SDA
{
	struct Comparator
	{
		virtual bool operator () (const int& val1, const int& val2) const
		{
			return false;
		}
	};

	struct Greater : public Comparator
	{
		bool operator () (const int& val1, const int& val2) const override
		{
			return val1 > val2;
		}
	};

	struct Less : public Comparator
	{
		bool operator () (const int& val1, const int& val2) const override
		{
			return val1 < val2;
		}
	};

	void BubbleSort(SDA::Vector<int>& vec, const Comparator& comp)
	{
		assert(vec.Size() >= 2);

		// just compare each element with the other and swap
		for (size_t i = 0; i < vec.Size(); ++i)
		{
			for (size_t j = 0; j < vec.Size(); ++j)
			{
				if ((i != j) && comp(vec[i], vec[j]))
				{
					SDA::Swap(vec[i], vec[j]);
				}
			}
		}
	}

	void SelectionSort(SDA::Vector<int>& vec, const Comparator& comp)
	{
		assert(vec.Size() >= 2);

		// One by one move boundary of unsorted subarray  
		for (size_t i = 0; i < vec.Size() - 1; ++i)
		{
			// Find the minimum or maximum element in the unsorted subarray
			size_t toSwapIdx = i;
			for (size_t j = i + 1; j < vec.Size(); ++j)
			{
				if (comp(vec[j], vec[toSwapIdx]))
				{
					toSwapIdx = j;
				}
			}

			// Swap the found minimum or maxmimum element with the first element
			if (toSwapIdx != i)
			{
				SDA::Swap(vec[toSwapIdx], vec[i]);
			}
		}
	}

	void InsertionSort(SDA::Vector<int>& vec, const Comparator& comp)
	{
		assert(vec.Size() >= 2);

		// unsorted subarray is on the right
		for (int i = 0; i < vec.Size(); ++i)
		{
			// store crr element
			int crr = vec[i];
			int j = i;

			// while comparison holds 
			while (j > 0 && comp(crr, vec[j - 1]))
			{
				// move the left element on the right
				vec[j] = vec[j - 1];
				j = j - 1;
			}
			vec[j] = crr;
		}
	}

	void CountSort(SDA::Vector<int>& vec, const Comparator& comp)
	{
		assert(vec.Size() >= 2);

		int max = SDA::Max(vec);
		int min = SDA::Min(vec);
		int range = max - min + 1;

		SDA::Vector<int> count(range), output(vec.Size());
		for (size_t i = 0; i < vec.Size(); i++)
			count[vec[i] - min]++;

		for (size_t i = 1; i < count.Size(); i++)
			count[i] += count[i - 1];

		for (size_t i = vec.Size() - 1; i >= 0; i--)
		{
			output[--count[vec[i] - min]] = vec[i];
		}

		for (int i = 0; i < vec.Size(); i++)
			vec[i] = output[i];
	}

	size_t Partition(SDA::Vector<int>& vec, int low, int high, const Comparator& comp)
	{
		int pivot = vec[high]; // we select the high index as pivot (last element)  
		int i = (low - 1); // Index of left most element  

		std::cout << "pivot: " << pivot << std::endl;

		// sort left subarray
		for (size_t j = low; j <= high - 1; ++j)
		{
			std::cout << "j: " << j << std::endl;

			if (comp(vec[j], pivot))
			{
				std::cout << "comparison holds" << std::endl;

				i++; // we increment the left most element while the comparison condition holds
				std::cout << "i: " << i << std::endl;

				if (i != j)
				{
					std::cout << "swap" << std::endl;
					SDA::Swap(vec[i], vec[j]);
				}
			}

			std::cout << vec;
		}
		std::cout << "swap pivot" << std::endl;
		// swap pivot with element after the last left most element
		SDA::Swap(vec[i + 1], vec[high]);

		std::cout << vec;

		return (i + 1);
	}

	// other info: https://www.geeksforgeeks.org/3-way-quicksort-dutch-national-flag/

	void QuickSort(SDA::Vector<int>& vec, int low, int high, const Comparator& comp)
	{
		assert(vec.Size() >= 2);

		if (low < high)
		{
			/* pi is partitioning index, vec[p] is now at right place */
			int pi = Partition(vec, low, high, comp);

			// Separately sort elements before  
			// partition and after partition  
			QuickSort(vec, low, pi - 1, comp);
			QuickSort(vec, pi + 1, high, comp);
		}
	}

	// Merges two subarrays of arr[]. 
	// First subarray is arr[l..m] 
	// Second subarray is arr[m+1..r] 
	void Merge(SDA::Vector<int>& vec, int left, int mid, int right, const Comparator& comp)
	{
		int i, j, k;
		int nLeft = mid - left + 1;
		int nRight = right - mid;

		/* create temp arrays */
		SDA::Vector<int> tempLeft, tempRight;
		tempLeft.Resize(nLeft);
		tempRight.Resize(nRight);

		/* Copy data to temp arrays */
		for (i = 0; i < nLeft; i++)
			tempLeft[i] = vec[left + i];
		for (j = 0; j < nRight; j++)
			tempRight[j] = vec[mid + 1 + j];

		/* Merge the temp arrays back into arr[l..r]*/
		i = 0; // Initial index of first subarray 
		j = 0; // Initial index of second subarray 
		k = left; // Initial index of merged subarray 
		while (i < nLeft && j < nRight)
		{
			if (comp(tempLeft[i], tempRight[j]))
			{
				vec[k] = tempLeft[i];
				i++;
			}
			else
			{
				vec[k] = tempRight[j];
				j++;
			}
			k++;
		}

		/* Copy the remaining elements of tempLeft, if there are any */
		while (comp(i, nLeft))
		{
			vec[k] = tempLeft[i];
			i++;
			k++;
		}

		/* Copy the remaining elements of tempRight, if there are any */
		while (comp(j, nRight))
		{
			vec[k] = tempRight[j];
			j++;
			k++;
		}
	}

	void MergeSort(SDA::Vector<int>& vec, int left, int right, const Comparator& comp)
	{
		assert(vec.Size() >= 2);

		if (left < right)
		{
			// Same as (left + right) / 2, but avoids overflow for large l and h 
			int mid = left + (right - left) / 2;

			// Separately sort first subarray
			// Separately sort second subarray
			MergeSort(vec, left, mid, comp);
			MergeSort(vec, mid + 1, right, comp);

			// merge the 2 subarrays into a final array
			Merge(vec, left, mid, right, comp);
		}
	}
}


#endif /* SORT_HPP */