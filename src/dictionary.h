#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <exception>
#include <utility>
#include <iostream>
//#include <vector>

template<typename Key, typename Value>
struct Node
{
    std::pair<Key, Value> pair_;
    Node<Key, Value> * left_branch_;
    Node<Key, Value> * right_branch_;
    Node()
    {
        left_branch_ = nullptr;
        right_branch_ = nullptr;
    }
    Node(const Key & key,  const Value & value)
    {
        pair_ = std::make_pair(key, value);
        left_branch_ = nullptr;
        right_branch_ = nullptr;
    }
};

template<typename Key, typename Value>
class dictionary
{
    public:
        virtual ~dictionary() = default;
        virtual const Value & get(const Key & key) const = 0;
        virtual void set(const Key & key,  const Value & value) = 0;
        virtual bool is_set(const Key & key) const = 0;
};

template<typename Key>
class not_found_exception : public std::exception
{
    public:
        not_found_exception() : exception() {}
        virtual const Key & get_key() const noexcept = 0;
};

template<typename Key>
class  map_exception : public not_found_exception<Key>
{
    private:
        Key key_;
    public:
        map_exception(Key key) : not_found_exception<Key>(), key_(key) {}
        const Key & get_key() const noexcept;

};

template<typename Key>
const Key & map_exception<Key>::get_key() const noexcept
{
    return key_;
}


template<typename Key_, typename Value_>
class my_map : public dictionary<Key_, Value_>
{
    private:
        Node<Key_, Value_> * root_;
        Node<Key_, Value_> * create_tree(Node<Key_, Value_> * node);
        void delete_all(Node<Key_, Value_> * & node);
        void deep_copy(Node<Key_, Value_> * & current, const Node<Key_, Value_> * other);
        int height(const Node<Key_, Value_> * node) const;
        void print_n(const Node<Key_, Value_> * node, int n, int level, int prob) const;
        void print(const Node<Key_, Value_> * node) const;
    public:
        my_map();
        my_map(const my_map & d);
        my_map(my_map && d);
        my_map & operator=(const my_map & d);
        my_map & operator=(my_map && d);
        ~my_map();
        const Value_ & get(const Key_ & key) const;
        void set(const Key_ & key,  const Value_ & value);
        void erase(Key_ && key);
        bool is_set(const Key_ & key) const;
        void print() const;
};

template<typename Key_, typename Value_>
void my_map<Key_, Value_>::deep_copy(Node<Key_, Value_> * & current, const Node<Key_, Value_> * other)
{
    if (current != nullptr && other != nullptr)
        {
            current->pair_ = other->pair_;
            deep_copy(current->left_branch_, other->left_branch_);
            deep_copy(current->right_branch_,other->right_branch_);
        }
    else if (current != nullptr && other == nullptr)
    {
        delete_all(current);
    }
    else if (current == nullptr && other != nullptr)
    {
        current = new Node<Key_, Value_>;
        current->pair_ = other->pair_;
        deep_copy(current->left_branch_, other->left_branch_);
        deep_copy(current->right_branch_, other->right_branch_);
    }
}

template<typename Key_, typename Value_>
my_map<Key_, Value_>::my_map() : dictionary<Key_, Value_>()
{
    root_ = nullptr;
}

template<typename Key_, typename Value_>
Node<Key_, Value_> * my_map<Key_, Value_>::create_tree(Node<Key_, Value_> * node)
{
    if (node == nullptr) return nullptr;

    Node<Key_, Value_> * new_node = new Node<Key_, Value_>(node->pair_.first, node->pair_.second);

    if (node->left_branch_ != nullptr)
        new_node->left_branch_ = create_tree(node->left_branch_);

    if (node->right_branch_ != nullptr)
        new_node->right_branch_ = create_tree(node->right_branch_);

    return new_node;
}

template<typename Key_, typename Value_>
my_map<Key_, Value_>::my_map(const my_map<Key_, Value_> & d) : dictionary<Key_, Value_>(d)
{
    root_ = create_tree(d.root_);
}

template<typename Key_, typename Value_>
my_map<Key_, Value_>::my_map(my_map<Key_, Value_> && d) : dictionary<Key_, Value_>(std::move(static_cast<dictionary<Key_, Value_>&>(d))), root_(d.root_)
{
    d.root_ = nullptr;
}

template<typename Key_, typename Value_>
my_map<Key_, Value_> & my_map<Key_, Value_>::operator=(const my_map<Key_, Value_> & d)
{
    if (this != &d)
    {
        deep_copy(this->root_, const_cast<const Node<Key_, Value_> *>(d.root_));
    }

    return *this;
}

template<typename Key_, typename Value_>
my_map<Key_, Value_> & my_map<Key_, Value_>::operator=(my_map<Key_, Value_> && d)
{
    std::swap(static_cast<dictionary<Key_, Value_>&>(*this), static_cast<dictionary<Key_, Value_>&>(d));
    std::swap(root_, d.root_);
    //delete_all(root_);
    //root_ = d.root_;
    //d.root_ = nullptr;

    return *this;
}

template<typename Key_, typename Value_>
void my_map<Key_, Value_>::delete_all(Node<Key_, Value_> * & node)
{
    if (node != nullptr)
    {
        Node<Key_, Value_> * l_branch = node->left_branch_;
        Node<Key_, Value_> * r_branch = node->right_branch_;

        delete node;
        node = nullptr;

        delete_all(l_branch);
        delete_all(r_branch);
    }
}

template<typename Key_, typename Value_>
my_map<Key_, Value_>::~my_map()
{
    delete_all(root_);
}


template<typename Key_, typename Value_>
const Value_ & my_map<Key_, Value_>::get(const Key_ & key) const // возврат значения
{
    auto current_node = root_;

    while(current_node != nullptr && current_node->pair_.first != key)
    {
        if (current_node->pair_.first > key)
        {
            current_node = current_node->left_branch_;
        }
        else
        {
            current_node = current_node->right_branch_;
        }
    }

    if (current_node == nullptr)
    {
        try
        {
            throw(map_exception<Key_>(key));
        }
        catch(map_exception<Key_> & ex)
        {
            std::cerr << "There isn`t element with the key " << ex.get_key() << std::endl;
        }
    }
    else
    {
        return current_node->pair_.second;
    }
}

template<typename Key_, typename Value_>
void my_map<Key_, Value_>::set(const Key_ & key,  const Value_ & value) // установка
{
    if (root_ == nullptr)
    {
        root_ = new Node<Key_, Value_>(key, value);
    }
    else
    {
        auto parent_node = root_;
        auto current_node = root_;

        while(current_node != nullptr && current_node->pair_.first != key)
        {
            parent_node = current_node;

            if (current_node->pair_.first > key)
            {
                current_node = current_node->left_branch_;
            }
            else
            {
                current_node = current_node->right_branch_;
            }
        }

        if (current_node == nullptr)
        {
            if (parent_node->pair_.first > key)
            {
                parent_node->left_branch_ = new Node<Key_, Value_>(key, value);
            }
            else
            {
                parent_node->right_branch_ = new Node<Key_, Value_>(key, value);
            }
        }
        else
        {
            std::cout << "Changed a value of the key " << key << std::endl;
            current_node->pair_.second = value;
        }
    }
}

template<typename Key_, typename Value_>
bool my_map<Key_, Value_>::is_set(const Key_ & key) const  // установлен ли
{
    auto current_node = root_;

    while(current_node != nullptr && current_node->pair_.first != key)
    {
        if (current_node->pair_.first > key)
        {
            current_node = current_node->left_branch_;
        }
        else
        {
            current_node = current_node->right_branch_;
        }
    }

    if (current_node == nullptr)
    {
        return false;
    }
    else
    {
        return true;
    }
}

template<typename Key_, typename Value_>
void my_map<Key_, Value_>::erase(Key_ && key)
{
    Node<Key_, Value_> ** current_node = &root_;

    Node<Key_, Value_> ** parent_node = &root_;

    while(current_node != nullptr && (*current_node)->pair_.first != key)
    {
        if ((*current_node)->pair_.first > key)
        {
            parent_node = current_node;
            current_node = &(*current_node)->left_branch_;
        }
        else
        {
            parent_node = current_node;
            current_node = &(*current_node)->right_branch_;
        }
    }

    if (current_node == nullptr)
    {
        std::cerr << "\nThere isn`t an element of the key " << key << std::endl;
    }
    else
    {
        // if it is a leaf
        if ((*current_node)->left_branch_ == nullptr && (*current_node)->right_branch_ == nullptr)
        {
            if ((*parent_node)->pair_.first > key)
            {
                delete (*parent_node)->left_branch_;
                (*parent_node)->left_branch_ = nullptr;
            }
            else
            {
                delete (*parent_node)->right_branch_;
                (*parent_node)->right_branch_ = nullptr;
            }
        }
        // if node has only right_branch
        else if ((*current_node)->left_branch_ == nullptr)
        {
            auto r_branch = (*current_node)->right_branch_;

            if ((*parent_node)->pair_.first > key)
            {
                delete (*parent_node)->left_branch_;
                (*parent_node)->left_branch_ = r_branch;
            }
            else
            {
                delete (*parent_node)->right_branch_;
                (*parent_node)->right_branch_ = r_branch;
            }
        }
        // if node has only left_branch
        else if ((*current_node)->right_branch_ == nullptr)
        {
            auto l_branch = (*current_node)->right_branch_;

            if ((*parent_node)->pair_.first > key)
            {
                delete (*parent_node)->left_branch_;
                (*parent_node)->left_branch_ = l_branch;
            }
            else
            {
                delete (*parent_node)->right_branch_;
                (*parent_node)->right_branch_ = l_branch;
            }
        }
        // there are 2 branches
        else
        {
            Node<Key_, Value_> * r_branch = (*current_node)->right_branch_;

            Node<Key_, Value_> * l_branch = (*current_node)->left_branch_;

            delete *current_node;

            // find min elem in the right subtree
            Node<Key_, Value_> * the_smallest_elem_in_right_branch = r_branch;

            Node<Key_, Value_> * parent_of_the_smallest_elem_in_right_branch = r_branch;

            while(the_smallest_elem_in_right_branch->left_branch_ != nullptr)
            {
                parent_of_the_smallest_elem_in_right_branch = the_smallest_elem_in_right_branch;
                the_smallest_elem_in_right_branch = the_smallest_elem_in_right_branch->left_branch_;
            }

            // add left_branch of delete node to the left_branch of new node of this place
            the_smallest_elem_in_right_branch->left_branch_ = l_branch;

            if (the_smallest_elem_in_right_branch != parent_of_the_smallest_elem_in_right_branch)
            {
                Node<Key_, Value_> * smallest_r_branch = the_smallest_elem_in_right_branch->right_branch_;

                // add right_node of the last smallest to  the left_branch of parent_node
                parent_of_the_smallest_elem_in_right_branch->left_branch_ = smallest_r_branch;

                // add right_branch of delete node to the rihth_branch of new node of this place
                the_smallest_elem_in_right_branch->right_branch_ = r_branch;
            }

            *current_node = the_smallest_elem_in_right_branch;
        }
    }
}

// funcs for print
template<typename Key_, typename Value_>
int my_map<Key_, Value_>::height(const Node<Key_, Value_> * node) const
{
    if (node == nullptr)
    {
        return 0;
    }

    int right_height = height(node->right_branch_);
    int left_height = height(node->left_branch_);

    if (right_height >= left_height)
    {
        return right_height + 1;
    }
    else
    {
        return left_height + 1;
    }
}

template<typename Key_, typename Value_>
void my_map<Key_, Value_>::print_n(const Node<Key_, Value_> * node, int n, int level, int prob) const
{
    if (node != nullptr)
    {
        if (level == n)
        {
            for (int i = 1; i <= prob; ++i)
                std::cout << ' ';
            std::cout << node-> pair_.first << ": " << node-> pair_.second;
        }
        else
        {
            print_n(node->left_branch_, n, level + 1, prob);
            print_n(node->right_branch_, n, level + 1, prob);
        }
    }
}

template<typename Key_, typename Value_>
void my_map<Key_, Value_>::print(const Node<Key_, Value_> * node) const
{
    int h = height(node);
    int prob = 4;

    if (node != nullptr)
    {
        for (int i = 0; i <= h; ++i)
        {
            print_n(node, i, 0, prob*(h - i));
            std::cout << std::endl;
        }
    }
}

template<typename Key_, typename Value_>
void my_map<Key_, Value_>::print() const
{
    print(root_);
}


#endif // DICTIONARY_H
