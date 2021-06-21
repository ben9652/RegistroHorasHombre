#pragma once

template<typename Vector>
class VectorIterator
{
public:
	using ValueType = typename Vector::ValueType;
	using PointerType = ValueType*;
	using ReferenceType = ValueType&;
public:
	VectorIterator(PointerType ptr)
		: m_Ptr(ptr) {}

	VectorIterator& operator++()
	{
		m_Ptr++;
		return *this;
	}

	VectorIterator operator++(int)
	{
		VectorIterator iterator = *this;
		++(*this);
		return iterator;
	}

	VectorIterator& operator--()
	{
		m_Ptr--;
		return *this;
	}

	VectorIterator operator--(int)
	{
		VectorIterator iterator = *this;
		--(*this);
		return iterator;
	}

	ReferenceType operator[](int index)
	{
		return *(m_Ptr + index);
	}

	PointerType operator->()
	{
		return m_Ptr;
	}

	ReferenceType operator*()
	{
		return *m_Ptr;
	}

	bool operator==(const VectorIterator& other) const
	{
		return m_Ptr == other.m_Ptr;
	}

	bool operator!=(const VectorIterator& other) const
	{
		return !(*this == other);
	}
private:
	PointerType m_Ptr;
};

template<typename T>
class Vector
{
public:
	using ValueType = T;
	using Iterator = VectorIterator<Vector<T>>;
private:
	T* m_Data = nullptr;

	size_t m_Size = 0;

	// Cu�nta memoria hemos reservado realmente.
	// No necesariamente memoria presente en el vector en el momento, sino cu�nto podr�amos almacenar sin tener que reasignar memoria.
	// �Por qu� tener esta variable? Resulta que haciendo un control de asignaciones de memoria, podremos ser capaces de no tener que reasignar memoria siempre cada vez que entre un nuevo elemento.
	size_t m_Capacity = 0;

	// Hay tres cosas, b�sicamente, que debe hacer esta funci�n.
	// 1. Asignar un nuevo bloque de memoria
	// 2. Necesitamos mover todos los elementos viejos al nuevo bloque de memoria.
	// 3. Borrar el bloque viejo de memoria.
	void ReAlloc(size_t newCapacity)
	{
		T* newBlock = (T*)::operator new(newCapacity * sizeof(T));

		// Si estamos disminuyendo el tama�o de nuestro vector, entonces reasignamos el tama�o m_Size al de newCapacity.
		if (newCapacity < m_Size)
			m_Size = newCapacity;

		for (size_t i = 0; i < m_Size; i++)
			new (&newBlock[i]) T(std::move(m_Data[i]));

		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();

		::operator delete(m_Data, m_Capacity * sizeof(T));
		m_Data = newBlock;
		m_Capacity = newCapacity;
	}

public:
	Vector()
	{
		// Esto lo que har� es reasignar la memoria
		ReAlloc(2);
	}

	~Vector()
	{
		Clear();
		::operator delete(m_Data, m_Capacity * sizeof(T));
	}

	void PushBack(const T& value)
	{
		// El problema aqu� es que, si no hay espacio para la asignaci�n del nuevo valor, deber�amos reasignar memoria.
		if (m_Size >= m_Capacity)
			ReAlloc(m_Capacity + m_Capacity / 2);

		m_Data[m_Size++] = value;
	}

	void PushBack(T&& value)
	{
		// El problema aqu� es que, si no hay espacio para la asignaci�n del nuevo valor, deber�amos reasignar memoria.
		if (m_Size >= m_Capacity)
			ReAlloc(m_Capacity + m_Capacity / 2);

		m_Data[m_Size++] = std::move(value);
	}

	template<typename... Args>
	T& EmplaceBack(Args&&... args)
	{
		if (m_Size >= m_Capacity)
			ReAlloc(m_Capacity + m_Capacity / 2);

		new(&m_Data[m_Size]) T(std::forward<Args>(args)...);

		return m_Data[m_Size++];
	}

	void PopBack()
	{
		if (m_Size > 0)
		{
			m_Size--;
			m_Data[m_Size].~T();
		}
	}

	void Clear()
	{
		for (size_t i = 0; i < m_Size; i++)
			m_Data[i].~T();

		m_Size = 0;
	}

	// Operador que puede acceder a un dato
	const T& operator[](size_t index) const
	{
		if (index >= m_Size)
		{
			// assert
		}

		return m_Data[index];
	}

	// Operador en su versi�n no constante en caso de que queramos modificar el elemento del vector
	T& operator[](size_t index)
	{
		return m_Data[index];
	}

	size_t Size() const { return m_Size; }

	Iterator begin()
	{
		return Iterator(m_Data);
	}

	Iterator end()
	{
		return Iterator(m_Data + m_Size);
	}
};