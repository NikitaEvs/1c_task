#include <iostream>
#include <vector>
#include <optional>
#include <fstream>
#include <cassert>

#include "json.hpp"
using json = nlohmann::json;

using UnderlyingType = std::uint8_t;
enum CardSuit : UnderlyingType {
  MinCard,
  Six,
  Seven,
  Eight,
  Nine,
  Ten,
  Jack,
  Lady,
  King,
  Ace
};

static constexpr UnderlyingType kFullDeskSize = static_cast<UnderlyingType>(CardSuit::Ace) -
    static_cast<UnderlyingType>(CardSuit::Six);

struct CardsChain {
  CardSuit top_;
  CardSuit bottom_;

  CardsChain() = default;
  explicit CardsChain(const std::string &name) {
    if (name == "Six") {
      top_ = bottom_ = CardSuit::Six;
    } else if (name == "Seven") {
      top_ = bottom_ = CardSuit::Seven;
    } else if (name == "Eight") {
      top_ = bottom_ = CardSuit::Eight;
    } else if (name == "Nine") {
      top_ = bottom_ = CardSuit::Nine;
    } else if (name == "Ten") {
      top_ = bottom_ = CardSuit::Ten;
    } else if (name == "Jack") {
      top_ = bottom_ = CardSuit::Jack;
    } else if (name == "Lady") {
      top_ = bottom_ = CardSuit::Lady;
    } else if (name == "King") {
      top_ = bottom_ = CardSuit::King;
    } else if (name == "Ace") {
      top_ = bottom_ = CardSuit::Ace;
    } else {
      top_ = bottom_ = CardSuit::MinCard;
    }
  }

  bool operator<(const CardsChain &rhs) const {
    return this->top_ < rhs.top_;
  }

  CardsChain& operator++() {
    UnsafeIncrement();
    assert(bottom_ != CardSuit::MinCard);

    return *this;
  }

  CardsChain& UnsafeIncrement() {
    bottom_ = static_cast<CardSuit>(static_cast<UnderlyingType>(bottom_) - 1);

    return *this;
  }

  [[nodiscard]] UnderlyingType GetSize() const {
    return static_cast<UnderlyingType>(top_) - static_cast<UnderlyingType>(bottom_);
  }
};

// 7 -> 6 - true
static bool isSequential(const CardsChain &first, const CardsChain &second) {
  auto firstCopy = first;
  firstCopy.UnsafeIncrement();

  if (firstCopy.bottom_ == CardSuit::MinCard) {
    return false;
  }

  return firstCopy.bottom_ == second.top_;
}

class Desk {
 public:
  using reference = CardsChain&;
  using const_reference = CardsChain const &;
  using iterator = std::vector<CardsChain>::iterator;
  using const_iterator = std::vector<CardsChain>::const_iterator;
  using size_type = std::vector<CardsChain>::size_type;

  iterator begin() noexcept {
    return desk_.begin();
  }

  iterator end() noexcept {
    return desk_.end();
  }

  [[nodiscard]] const_iterator begin() const noexcept {
    return desk_.begin();
  }

  [[nodiscard]] const_iterator end() const noexcept {
    return desk_.end();
  }

  [[nodiscard]] size_type size() const noexcept {
    return desk_.size();
  }

  [[nodiscard]] bool empty() const noexcept {
    return desk_.empty();
  }

  reference back() noexcept {
    return desk_.back();
  }

  [[nodiscard]] const_reference back() const noexcept {
    return desk_.back();
  }

  bool operator<(const Desk &rhs) const {
    if (rhs.empty()) {
      return true;
    } else if (this->empty()) {
      return false;
    } else {
      return this->back() < rhs.back();
    }
  }

  void AddNewChain(CardsChain chain) {
    desk_.emplace_back(chain);
  }

  void RemoveLastChain() {
    if (!desk_.empty()) {
      desk_.pop_back();
    }
  }

  void AddToChain() {
    if (!desk_.empty()) {
      ++desk_.back();
    }
  }

  void Normalize() {
    for (size_type i = 1; i < desk_.size(); ++i) {
      auto& prev_card_chain = desk_[i - 1];
      auto& current_card_chain = desk_[i];

      if (isSequential(prev_card_chain, current_card_chain)) {
        ++prev_card_chain;
        desk_.erase(desk_.begin() + static_cast<ptrdiff_t>(i));
        --i;
      }
    }
  }

 private:
  std::vector<CardsChain> desk_;
};

// first.back() -> second.back()
static bool isSequential(const Desk &first, const Desk &second) {
  if (first.empty() || second.empty()) {
    return true;
  }
  return isSequential(second.back(), first.back());
}

class GameField {
 public:
  using iterator = std::vector<Desk>::iterator;
  using const_iterator = std::vector<Desk>::const_iterator;
  using size_type = Desk::size_type;

  iterator begin() noexcept {
    return map_.begin();
  }

  iterator end() noexcept {
    return map_.end();
  }

  [[nodiscard]] const_iterator begin() const noexcept {
    return map_.begin();
  }

  [[nodiscard]] const_iterator end() const noexcept {
    return map_.end();
  }

  [[nodiscard]] size_type size() const noexcept {
    size_type size = 0;

    for (const auto& vec : map_) {
      size += vec.size();
    }

    return size;
  }

  [[nodiscard]] bool empty() const noexcept {
    return size() == 0;
  }

  friend std::istream& operator>>(std::istream &in, GameField &field) {
    json input_json;
    in >> input_json;

    for (const auto& desk_description : input_json) {
      Desk new_desk;
      for (const std::string card_description : desk_description) {
        CardsChain new_card(card_description);
        new_desk.AddNewChain(new_card);
      }

      field.map_.emplace_back(std::move(new_desk));
    }

    return in;
  }

  friend std::ostream& operator<<(std::ostream &out, const GameField &field) {
    for (const auto& desk : field.map_) {
      for (const auto& card : desk) {
        out << static_cast<int>(card.top_) << "->" << static_cast<int>(card.bottom_) << ' ';
      }
      out << std::endl;
    }

    return out;
  }

  void Normalize() {
    for (auto& desk : map_) {
      desk.Normalize();

      if (!desk.empty() && desk.back().GetSize() == kFullDeskSize) {
        desk.RemoveLastChain();
      }
    }
  }

  bool IsWinning() {
    Normalize();

    while (!empty()) {
      SortDesks();
      if (!NextStep()) {
        return false;
      }
    }

    return true;
  }

 private:
  std::vector<Desk> map_;

  void SortDesks() {
    std::sort(map_.begin(), map_.end());
  }

  bool NextStep() {
    for (std::size_t i = 1; i < map_.size(); ++i) {
      auto& prev_desk = map_[i - 1];
      auto& current_desk = map_[i];

      if (isSequential(prev_desk, current_desk)) {
        current_desk.AddToChain();
        prev_desk.RemoveLastChain();

        if (!current_desk.empty() && (current_desk.back().GetSize() == kFullDeskSize)) {
          current_desk.RemoveLastChain();
        }

        return true;
      }
    }

    return false;
  }
};
