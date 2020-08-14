#ifndef MAGMATIC_DGRAPH_HPP
#define MAGMATIC_DGRAPH_HPP

#include <vector>
#include <cassert>
#include <compare>
#include <stdexcept>

template<typename T>
class DGraph
{
public:
	template<bool isConst>
	class Node;

	template<bool isConst>
	class Iterator;

	using value_type            = T;
	using reference_type        = Node<false>;
	using const_reference_type  = Node<true>;

	using iterator              = Iterator<false>;
	using const_iterator        = Iterator<true>;

	using reverse_iterator      = std::reverse_iterator<iterator>;
	using const_reverse_iterator= std::reverse_iterator<const_iterator>;

	using size_type = std::size_t;


	template<bool isConst>
	class Node
	{
	public:
		[[nodiscard]] T* operator->() noexcept
		{
			checkIdValid();
			return &(dgraph_.nodes_[index_]);
		}

		[[nodiscard]] const T* operator->() const noexcept
		{
			checkIdValid();
			return &(dgraph_.nodes_[index_]);
		}

		[[nodiscard]] T& operator*() noexcept
		{
			checkIdValid();
			return dgraph_->nodes_[index_];
		}

		[[nodiscard]] const T& operator*() const noexcept
		{
			checkIdValid();
			return dgraph_->nodes_[index_];
		}

		[[nodiscard]] std::size_t id() const noexcept
		{
			return index_;
		}

	private:
		using dgraph_pointer_t = std::conditional_t<
		        isConst,
		        const DGraph*,
		        DGraph*>;

		Node(dgraph_pointer_t dgraph, std::size_t index) noexcept
		: dgraph_(dgraph), index_(index)
		{};

		dgraph_pointer_t dgraph_;
		std::size_t index_;

		void checkIdValid() const noexcept
		{
			assert(index_ < dgraph_->nodes_.size());
		}

		friend class DGraph;
	};

	template<bool isConst>
	class Iterator
	{
	public:
		using self_type = Iterator;
		using value_type = std::conditional_t<
				isConst,
				DGraph::const_reference_type,
				DGraph::reference_type
				>;
		using reference = value_type ;
		using pointer = void;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::random_access_iterator_tag;

		Iterator()
		:dgraph_(nullptr), index_(0)
		{};

		operator DGraph::const_iterator() const noexcept
		{
			return DGraph::const_iterator(dgraph_, index_);
		}

		[[nodiscard]] reference operator*() const noexcept
		{
			return value_type(dgraph_, index_);
		}

		[[nodiscard]] bool operator==(const self_type& other) const noexcept
		{
			return dgraph_ == other.dgraph_ && index_ == other.index_;
		}

		[[nodiscard]] bool operator!=(const self_type& other) const noexcept
		{
			return !(*this == other);
		}

		[[nodiscard]] auto operator<=>(const self_type& other) const noexcept
		{
			return other.index_ <=> index_;
		}

		self_type& operator++() noexcept
		{
			++index_;
			return *this;
		}
		self_type& operator--() noexcept
		{
			--index_;
			return *this;
		}
		self_type operator++(int) noexcept
		{
			const auto temp = *this;
			++index_;
			return temp;
		}
		self_type operator--(int) noexcept
		{
			const auto temp = *this;
			--index_;
			return temp;
		}

		self_type& operator+=(std::size_t shift) noexcept
		{
			index_ += shift;
			return *this;
		}
		self_type& operator-=(std::size_t shift) noexcept
		{
			index_ -= shift;
			return *this;
		}

		[[nodiscard]] self_type operator+(std::size_t shift) const noexcept
		{
			return self_type(dgraph_, index_ + shift);
		}
		[[nodiscard]] self_type operator-(std::size_t shift) const noexcept
		{
			return self_type(dgraph_, index_ - shift);
		}

		[[nodiscard]] std::size_t operator-(const self_type& rhs) const noexcept
		{
			return index_ - rhs.index_;
		}

	private:
		using dgraph_pointer_t = std::conditional_t<
		        isConst,
		        const DGraph*,
		        DGraph*>;

		dgraph_pointer_t dgraph_;
		std::size_t index_;

		Iterator(dgraph_pointer_t dgraph, size_type index)
		:dgraph_(dgraph), index_(index)
		{}

		friend class DGraph;
	};

	DGraph() noexcept = default;

	DGraph(const DGraph& d_graph) = default;
	DGraph& operator=(const DGraph& d_graph) = default;

	DGraph(DGraph && d_graph) noexcept = default;
	DGraph& operator=(DGraph && d_graph) noexcept = default;

	~DGraph() noexcept = default;

	// iterators

	iterator begin() noexcept
	{
		return iterator(this, 0);
	}
	const_iterator begin() const noexcept
	{
		return const_iterator(this, 0);
	}
	const_iterator cbegin() const noexcept
	{
		return const_iterator(this, 0);
	}

	iterator end() noexcept {
		return iterator(this, nodes_.size());
	}
	const_iterator end() const noexcept {
		return const_iterator(this, nodes_.size());
	}
	const_iterator cend() const noexcept {
		return const_iterator(this, nodes_.size());
	}

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}
	const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}
	const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}
	const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(begin());
	}
	const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(begin());
	}

	// info

	[[nodiscard]] size_type size() const noexcept
	{
		return nodes_.size();
	}

	[[nodiscard]] size_type max_size() const noexcept
	{
		return nodes_.max_size();
	}

	[[nodiscard]] bool empty() const noexcept
	{
		return nodes_.empty();
	}

	// access

	[[nodiscard]] reference_type operator[](size_type i) noexcept
	{
		return reference_type(this, i);
	}
	[[nodiscard]] const_reference_type operator[](size_type i) const noexcept
	{
		return const_reference_type(this, i);
	}
	reference_type at(size_type i)
	{
		rangeCheck(i);
		return reference_type(this, i);
	}
	const_reference_type at(size_type i) const
	{
		rangeCheck(i);
		return const_reference_type(this, i);
	}

	[[nodiscard]] reference_type front() noexcept
	{
		return reference_type(this, 0);
	}
	[[nodiscard]] const_reference_type front() const noexcept
	{
		return const_reference_type(this, 0);
	}
	[[nodiscard]] reference_type back() noexcept
	{
		return reference_type(this, size()-1);
	}
	[[nodiscard]] const_reference_type back() const noexcept
	{
		return const_reference_type(this, size()-1);
	}

	// modifiers

	void clear() noexcept
	{
		nodes_.clear();
		edges_end_node_.clear();
		edges_range_.clear();
	}

	template<typename... Args>
	reference_type emplace_back(Args&&... args)
	requires std::constructible_from<T, Args...>
	{
		nodes_.emplace_back(std::forward<Args>(args)...);
		edges_range_.emplace_back(edges_end_node_.size());

		return reference_type(this, nodes_.size()-1);
	}
	void push_back(const T& value)
	{
		nodes_.emplace_back(value);
		edges_range_.emplace_back(edges_end_node_.size());
	}
	void push_back(T && value)
	{
		emplace_back(std::move(value));
	}
	void pop_back() noexcept
	{
		nodes_.pop_back();
		const auto index = nodes_.size();
		if(index == 0)
		{
			edges_end_node_.clear();
			edges_range_.clear();
			return;
		}
		if(edges_range_[index] != edges_range_[index-1])
		{
			nodes_.resize(edges_range_[index-1]);
		}
	}

private:
	using NodeEdgesMap = std::vector<std::size_t>;

	std::vector<T> nodes_;
	std::vector<size_type> edges_end_node_;
	std::vector<size_type> edges_range_;

	void rangeCheck(size_type index) const
	{
		if(index >= nodes_.size())
		{
			throw std::out_of_range("");
		}
	}
};


#endif //MAGMATIC_DGRAPH_HPP
