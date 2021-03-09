class Trie {
  constructor() {
    this.head = {}
  }

  validate(word) {
    if (word == undefined) throw 'Word is undefined or null.'
    if (typeof word !== 'string') throw 'Given word is not a string.'
  }

  insert(word) {
    this.validate(word)

    // Using this to navigate through trie
    let current = this.head

    // For each letter in word, go to letter
    for (const ch of word) {
      // If letter doesn't exist it, create it
      if (!ch in current) current[ch] = {}

      // If another letter, return to previous step
      // Otherwise add a word terminating marker
      current = current[ch]
    }

    // Word terminating marker
    current.$ = true
  }

  hasWord(word) {
    this.validate(word)

    // Use this pointer to navigate through tree
    let current = this.head

    for (const ch of word) {
      // If letter doesn't exist, return false
      if (!ch in current) return false

      // If letter exists, go to letter
      current = current[ch]
    }

    // Word terminating marker
    return current.$ === true
  }

  delete(word) {}
}
