// Saints and Feast Days Database for Catholic Liturgical Calendar
// Returns saint/feast information for any given date

const dateInfo = $json; // Expecting dateInfo.monthDay format like "8/24"

const saintsDatabase = {
  // January
  '1/1': { name: 'Mary, Mother of God', type: 'Solemnity', quote: 'My soul proclaims the greatness of the Lord.', intro: 'The Solemnity of Mary, the Holy Mother of God, celebrates Mary\'s divine motherhood and her role in salvation history.' },
  '1/2': { name: 'Basil the Great and Gregory Nazianzen', type: 'Memorial', quote: 'We must prefer nothing whatever to Christ.', intro: 'These two fourth-century bishops and theologians were close friends who defended the divinity of Christ against the Arian heresy.' },
  '1/3': { name: 'Most Holy Name of Jesus', type: 'Optional Memorial', quote: 'At the name of Jesus every knee should bow.', intro: 'This feast honors the name of Jesus, which means "God saves," reminding us of the power in Christ\'s holy name.' },
  '1/4': { name: 'Elizabeth Ann Seton', type: 'Memorial', quote: 'The first end I propose in our daily work is to do the will of God.', intro: 'First native-born American saint, she founded the first American religious community for women and Catholic schools in America.' },
  '1/5': { name: 'John Neumann', type: 'Memorial', quote: 'A man must always be ready, for death comes when and where God wills it.', intro: 'Fourth Bishop of Philadelphia, he established the first Catholic diocesan school system in the United States.' },
  '1/6': { name: 'Epiphany of the Lord', type: 'Solemnity', quote: 'We have seen his star at its rising and have come to do him homage.', intro: 'The Epiphany celebrates the manifestation of Christ to the Gentiles, represented by the Magi.' },
  '1/7': { name: 'Raymond of Penyafort', type: 'Optional Memorial', quote: 'The law of the Church is the law of love.', intro: 'A Dominican priest who compiled the Decretals of Gregory IX and was a patron of canon lawyers.' },
  '1/13': { name: 'Hilary of Poitiers', type: 'Optional Memorial', quote: 'Everything that seems empty is full of the angels of God.', intro: 'Bishop and Doctor of the Church who defended orthodox Christianity against Arianism in the West.' },
  '1/17': { name: 'Anthony, Abbot', type: 'Memorial', quote: 'Our life and our death is with our neighbor.', intro: 'Father of Christian monasticism who spent 20 years alone in the Egyptian desert.' },
  '1/20': { name: 'Fabian', type: 'Optional Memorial', quote: 'The Church grows through the blood of martyrs.', intro: 'Pope and martyr who led the Church for 14 years during a time of peace before his martyrdom.' },
  '1/21': { name: 'Agnes', type: 'Memorial', quote: 'Christ is my spouse; he has chosen me.', intro: 'Virgin martyr who died at age 12 or 13 for refusing to marry, choosing consecration to Christ.' },
  '1/22': { name: 'Vincent', type: 'Optional Memorial', quote: 'You may torture this body, but you cannot touch my soul.', intro: 'Deacon and martyr from Spain who remained faithful during Diocletian\'s persecution.' },
  '1/24': { name: 'Francis de Sales', type: 'Memorial', quote: 'Be patient with everyone, but above all with yourself.', intro: 'Bishop of Geneva, Doctor of the Church, known for his gentleness and writings on spiritual life.' },
  '1/25': { name: 'Conversion of Paul', type: 'Feast', quote: 'It is no longer I who live, but Christ who lives in me.', intro: 'Celebrates Paul\'s dramatic conversion on the road to Damascus from persecutor to apostle.' },
  '1/26': { name: 'Timothy and Titus', type: 'Memorial', quote: 'Fight the good fight of faith.', intro: 'Companions of St. Paul and first-generation bishops who helped establish the early Church.' },
  '1/27': { name: 'Angela Merici', type: 'Optional Memorial', quote: 'Have patience and remain wide awake. God wills it.', intro: 'Founded the Ursulines, the first teaching order of women in the Church.' },
  '1/28': { name: 'Thomas Aquinas', type: 'Memorial', quote: 'To one who has faith, no explanation is necessary. To one without faith, no explanation is possible.', intro: 'Dominican priest, Doctor of the Church, greatest medieval theologian and philosopher.' },
  '1/31': { name: 'John Bosco', type: 'Memorial', quote: 'Run, jump, shout, but do not sin.', intro: 'Founded the Salesians to educate poor youth, pioneering preventive system of education.' },

  // February
  '2/2': { name: 'Presentation of the Lord', type: 'Feast', quote: 'Now you dismiss your servant, O Lord, according to your word in peace.', intro: 'Commemorates the presentation of Jesus in the Temple and the purification of Mary.' },
  '2/3': { name: 'Blaise', type: 'Optional Memorial', quote: 'May God deliver you from every evil of the throat and from every other evil.', intro: 'Bishop and martyr, patron of throat ailments, traditionally invoked for blessing of throats.' },
  '2/5': { name: 'Agatha', type: 'Memorial', quote: 'Jesus Christ, Lord of all, you see my heart.', intro: 'Virgin martyr from Sicily who suffered torture rather than renounce her faith.' },
  '2/6': { name: 'Paul Miki and Companions', type: 'Memorial', quote: 'I give thanks to God that I can die as a Christian.', intro: 'Twenty-six martyrs crucified in Nagasaki, Japan, including Jesuits, Franciscans, and laypeople.' },
  '2/8': { name: 'Jerome Emiliani', type: 'Optional Memorial', quote: 'Jesus Christ, our teacher and guide, you are the way, the truth, and the life.', intro: 'Founded the Somaschi Fathers to care for orphans and the education of youth.' },
  '2/10': { name: 'Scholastica', type: 'Memorial', quote: 'He can do more whose love is greater.', intro: 'Twin sister of St. Benedict, foundress of Benedictine nuns.' },
  '2/11': { name: 'Our Lady of Lourdes', type: 'Optional Memorial', quote: 'I am the Immaculate Conception.', intro: 'Commemorates the apparitions of Mary to Bernadette Soubirous at Lourdes, France.' },
  '2/14': { name: 'Cyril and Methodius', type: 'Memorial', quote: 'Go therefore and make disciples of all nations.', intro: 'Brothers who evangelized the Slavic peoples and created the Cyrillic alphabet.' },
  '2/17': { name: 'Seven Founders of the Servite Order', type: 'Optional Memorial', quote: 'We are servants of Mary, the Mother of God.', intro: 'Seven Florentine merchants who withdrew to serve Mary and founded the Servites.' },
  '2/21': { name: 'Peter Damian', type: 'Optional Memorial', quote: 'The hermitage is a school of heavenly doctrine.', intro: 'Cardinal, Doctor of the Church, reformer of Church discipline and clerical life.' },
  '2/22': { name: 'Chair of Peter', type: 'Feast', quote: 'You are Peter, and upon this rock I will build my Church.', intro: 'Celebrates the authority Christ gave to Peter and his successors.' },
  '2/23': { name: 'Polycarp', type: 'Memorial', quote: 'Eighty-six years I have served Him, and He has done me no wrong.', intro: 'Bishop of Smyrna, disciple of John the Apostle, martyred at age 86.' },

  // March
  '3/4': { name: 'Casimir', type: 'Optional Memorial', quote: 'I would rather die than offend God by sin.', intro: 'Polish prince who renounced the throne to live a life of celibacy and devotion.' },
  '3/7': { name: 'Perpetua and Felicity', type: 'Memorial', quote: 'Stand fast in the faith.', intro: 'Noble woman and her slave, martyred together in Carthage, leaving a powerful diary.' },
  '3/8': { name: 'John of God', type: 'Optional Memorial', quote: 'Work hard, brothers, for love of God.', intro: 'Founded the Brothers Hospitallers to care for the sick and poor.' },
  '3/9': { name: 'Frances of Rome', type: 'Optional Memorial', quote: 'A married woman must leave God at the altar to find Him in her housekeeping.', intro: 'Married woman who founded a religious community while caring for her family.' },
  '3/17': { name: 'Patrick', type: 'Optional Memorial', quote: 'Christ with me, Christ before me, Christ behind me.', intro: 'Patron of Ireland who evangelized the Irish people in the 5th century.' },
  '3/18': { name: 'Cyril of Jerusalem', type: 'Optional Memorial', quote: 'The dragon sits by the side of the road, watching those who pass.', intro: 'Bishop and Doctor of the Church, known for his catechetical instructions.' },
  '3/19': { name: 'Joseph', type: 'Solemnity', quote: 'Joseph, son of David, do not be afraid to take Mary as your wife.', intro: 'Foster father of Jesus, patron of the universal Church and workers.' },
  '3/23': { name: 'Turibius of Mogrovejo', type: 'Optional Memorial', quote: 'Christ is my life and death is my gain.', intro: 'Archbishop of Lima who reformed the Church in Peru and defended indigenous peoples.' },
  '3/25': { name: 'Annunciation', type: 'Solemnity', quote: 'Let it be done unto me according to your word.', intro: 'Celebrates the angel Gabriel\'s announcement to Mary and the Incarnation.' },

  // April
  '4/2': { name: 'Francis of Paola', type: 'Optional Memorial', quote: 'Charity is the most excellent way to God.', intro: 'Hermit and founder of the Minims, known for humility and miracles.' },
  '4/4': { name: 'Isidore of Seville', type: 'Optional Memorial', quote: 'Learning unsupported by grace may get into our ears; it never reaches the heart.', intro: 'Bishop and Doctor of the Church, last of the Latin Fathers.' },
  '4/5': { name: 'Vincent Ferrer', type: 'Optional Memorial', quote: 'Fear not! Tomorrow Christ will come.', intro: 'Dominican preacher who helped end the Western Schism.' },
  '4/7': { name: 'John Baptist de la Salle', type: 'Memorial', quote: 'Remember you are in the holy presence of God.', intro: 'Founded the Christian Brothers and pioneered education for the poor.' },
  '4/11': { name: 'Stanislaus', type: 'Memorial', quote: 'It is better to suffer for justice than to reign by injustice.', intro: 'Bishop of Krakow martyred by the king for defending Church rights.' },
  '4/13': { name: 'Martin I', type: 'Optional Memorial', quote: 'May God forgive them, for they know not what they do.', intro: 'Pope and martyr who defended orthodox Christology against Monothelitism.' },
  '4/21': { name: 'Anselm', type: 'Optional Memorial', quote: 'God is that than which nothing greater can be conceived.', intro: 'Archbishop of Canterbury, Doctor of the Church, father of Scholasticism.' },
  '4/23': { name: 'George', type: 'Optional Memorial', quote: 'I am a soldier of Christ; I cannot fight.', intro: 'Soldier martyr, patron of England, symbol of Christian chivalry.' },
  '4/24': { name: 'Fidelis of Sigmaringen', type: 'Optional Memorial', quote: 'Woe is me if I do not preach the Gospel!', intro: 'Capuchin friar and martyr during the Counter-Reformation.' },
  '4/25': { name: 'Mark', type: 'Feast', quote: 'Go into all the world and preach the Gospel to every creature.', intro: 'Evangelist, companion of Peter and Paul, author of the second Gospel.' },
  '4/28': { name: 'Peter Chanel', type: 'Optional Memorial', quote: 'You may kill me, but my religion will not die.', intro: 'First martyr of Oceania, missionary to Futuna Island.' },
  '4/29': { name: 'Catherine of Siena', type: 'Memorial', quote: 'Be who God meant you to be and you will set the world on fire.', intro: 'Doctor of the Church, mystic who helped return the papacy to Rome.' },
  '4/30': { name: 'Pius V', type: 'Optional Memorial', quote: 'Lord, increase my sufferings and my love for You.', intro: 'Pope who implemented the Council of Trent and reformed the liturgy.' },

  // May
  '5/1': { name: 'Joseph the Worker', type: 'Optional Memorial', quote: 'Work is a blessing from God.', intro: 'Honors St. Joseph as patron of workers and the dignity of human labor.' },
  '5/2': { name: 'Athanasius', type: 'Memorial', quote: 'God became man so that man might become God.', intro: 'Bishop of Alexandria, Doctor of the Church, defender of Christ\'s divinity.' },
  '5/3': { name: 'Philip and James', type: 'Feast', quote: 'Come and see!', intro: 'Two of the twelve apostles who spread the Gospel.' },
  '5/10': { name: 'John of Avila', type: 'Optional Memorial', quote: 'We must ask God to give us what we need to serve Him.', intro: 'Mystic, preacher, spiritual director to Teresa of Avila and John of the Cross.' },
  '5/12': { name: 'Nereus and Achilleus', type: 'Optional Memorial', quote: 'We are soldiers of Christ, we cannot shed blood.', intro: 'Roman soldiers who converted and were martyred for the faith.' },
  '5/13': { name: 'Our Lady of Fatima', type: 'Optional Memorial', quote: 'Pray the Rosary every day to obtain peace for the world.', intro: 'Commemorates Mary\'s apparitions to three shepherd children in Portugal.' },
  '5/14': { name: 'Matthias', type: 'Feast', quote: 'You did not choose me, but I chose you.', intro: 'Chosen to replace Judas Iscariot among the twelve apostles.' },
  '5/18': { name: 'John I', type: 'Optional Memorial', quote: 'Better to die for the truth than to live in error.', intro: 'Pope and martyr who died in prison under the Arian king Theodoric.' },
  '5/20': { name: 'Bernardine of Siena', type: 'Optional Memorial', quote: 'The name of Jesus is the glory of preachers.', intro: 'Franciscan preacher who promoted devotion to the Holy Name of Jesus.' },
  '5/21': { name: 'Christopher Magallanes', type: 'Optional Memorial', quote: 'I die innocent, and ask God that my blood may serve to bring peace to our divided Mexico.', intro: 'Mexican priest martyred with companions during the Cristero War.' },
  '5/22': { name: 'Rita of Cascia', type: 'Optional Memorial', quote: 'Nothing is impossible with God.', intro: 'Wife, mother, widow, and nun, patron of impossible causes.' },
  '5/25': { name: 'Bede the Venerable', type: 'Optional Memorial', quote: 'Christ is the morning star who when the night of this world is past brings to his saints the promise of the light of life.', intro: 'English monk, Doctor of the Church, father of English history.' },
  '5/26': { name: 'Philip Neri', type: 'Memorial', quote: 'Cast yourself into the arms of God and be very sure that if He wants anything of you, He will fit you for the work and give you strength.', intro: 'Founded the Oratory, known as the "Apostle of Rome" for his joyful holiness.' },
  '5/27': { name: 'Augustine of Canterbury', type: 'Optional Memorial', quote: 'Christ has sent me to you, and I bring you joyful news.', intro: 'First Archbishop of Canterbury, evangelized the English.' },
  '5/31': { name: 'Visitation', type: 'Feast', quote: 'Blessed are you among women, and blessed is the fruit of your womb!', intro: 'Celebrates Mary\'s visit to Elizabeth and the Magnificat.' },

  // June
  '6/1': { name: 'Justin Martyr', type: 'Memorial', quote: 'We would rather die than live a lie.', intro: 'Philosopher who became Christian apologist, first to use philosophy to explain faith.' },
  '6/2': { name: 'Marcellinus and Peter', type: 'Optional Memorial', quote: 'Christ is our strength and our crown.', intro: 'Priest and exorcist martyred during Diocletian\'s persecution.' },
  '6/3': { name: 'Charles Lwanga and Companions', type: 'Memorial', quote: 'Tell the king that he has put us to death for nothing.', intro: 'Ugandan martyrs who died rather than submit to the immoral demands of their king.' },
  '6/5': { name: 'Boniface', type: 'Memorial', quote: 'Come, brother, be of strong heart in Christ.', intro: 'English missionary, "Apostle of Germany," martyred while evangelizing.' },
  '6/6': { name: 'Norbert', type: 'Optional Memorial', quote: 'Jesus Christ, and him crucified.', intro: 'Founded the Premonstratensians, reformed clergy, bishop of Magdeburg.' },
  '6/9': { name: 'Ephrem', type: 'Optional Memorial', quote: 'Every tongue should praise Him, and every mouth should confess Him.', intro: 'Deacon, Doctor of the Church, Syrian theologian and hymn writer.' },
  '6/11': { name: 'Barnabas', type: 'Memorial', quote: 'Do not lose heart. We must pass through many trials to enter the kingdom of God.', intro: 'Companion of Paul, called an apostle though not one of the Twelve.' },
  '6/13': { name: 'Anthony of Padua', type: 'Memorial', quote: 'Actions speak louder than words; let your words teach and your actions speak.', intro: 'Franciscan priest, Doctor of the Church, powerful preacher and miracle worker.' },
  '6/19': { name: 'Romuald', type: 'Optional Memorial', quote: 'Sit in your cell as in paradise.', intro: 'Founded the Camaldolese monks, combining eremitical and cenobitic life.' },
  '6/21': { name: 'Aloysius Gonzaga', type: 'Memorial', quote: 'It is better to be a child of God than king of the whole world.', intro: 'Jesuit seminarian who died caring for plague victims, patron of youth.' },
  '6/22': { name: 'Paulinus of Nola', type: 'Optional Memorial', quote: 'Christ is our song.', intro: 'Senator who became bishop, known for Christian poetry.' },
  '6/24': { name: 'Birth of John the Baptist', type: 'Solemnity', quote: 'Prepare the way of the Lord, make straight his paths.', intro: 'Celebrates the birth of Christ\'s forerunner who prepared the way.' },
  '6/27': { name: 'Cyril of Alexandria', type: 'Optional Memorial', quote: 'Mary is the Mother of Emmanuel, truly the Mother of God.', intro: 'Bishop, Doctor of the Church, defended Mary as Mother of God.' },
  '6/28': { name: 'Irenaeus', type: 'Memorial', quote: 'The glory of God is man fully alive.', intro: 'Bishop of Lyon, early Church Father who fought Gnosticism.' },
  '6/29': { name: 'Peter and Paul', type: 'Solemnity', quote: 'I have fought the good fight, I have finished the race, I have kept the faith.', intro: 'The two great apostles, pillars of the Church, both martyred in Rome.' },
  '6/30': { name: 'First Martyrs of Rome', type: 'Optional Memorial', quote: 'We would rather die than deny Christ.', intro: 'Christians killed by Nero after the great fire of Rome in 64 AD.' },

  // July
  '7/1': { name: 'Junipero Serra', type: 'Optional Memorial', quote: 'Always go forward, never turn back.', intro: 'Franciscan missionary who founded the California missions.' },
  '7/3': { name: 'Thomas', type: 'Feast', quote: 'My Lord and my God!', intro: 'Apostle whose doubt led to profound faith, traditionally evangelized India.' },
  '7/4': { name: 'Elizabeth of Portugal', type: 'Optional Memorial', quote: 'Blessed are the peacemakers, for they shall be called children of God.', intro: 'Queen who was a peacemaker and served the poor.' },
  '7/5': { name: 'Anthony Zaccaria', type: 'Optional Memorial', quote: 'Be on fire with love for Jesus Christ.', intro: 'Founded the Barnabites to reform the clergy.' },
  '7/6': { name: 'Maria Goretti', type: 'Optional Memorial', quote: 'I forgive him, and I want him to be with me in Paradise.', intro: 'Virgin martyr who forgave her attacker, patron of youth and purity.' },
  '7/9': { name: 'Augustine Zhao Rong and Companions', type: 'Optional Memorial', quote: 'It is a joyful thing to suffer for our Lord.', intro: 'Chinese martyrs spanning three centuries of persecution.' },
  '7/11': { name: 'Benedict', type: 'Memorial', quote: 'Prefer nothing to the love of Christ.', intro: 'Father of Western monasticism, wrote the Rule that shaped religious life.' },
  '7/13': { name: 'Henry', type: 'Optional Memorial', quote: 'Christ must reign in our hearts and in our kingdoms.', intro: 'Holy Roman Emperor who used power to reform the Church.' },
  '7/14': { name: 'Kateri Tekakwitha', type: 'Memorial', quote: 'I am not my own; I have given myself to Jesus.', intro: 'First Native American saint, "Lily of the Mohawks."' },
  '7/15': { name: 'Bonaventure', type: 'Memorial', quote: 'In beautiful things he saw Beauty itself.', intro: 'Franciscan theologian, Doctor of the Church, minister general of Franciscans.' },
  '7/16': { name: 'Our Lady of Mount Carmel', type: 'Optional Memorial', quote: 'Take this scapular, whosoever dies wearing it shall not suffer eternal fire.', intro: 'Honors Mary as patroness of Carmelites and the Brown Scapular devotion.' },
  '7/18': { name: 'Camillus de Lellis', type: 'Optional Memorial', quote: 'Even if I had a thousand lives, I would dedicate them all to the service of the sick.', intro: 'Reformed soldier who founded order to care for the sick.' },
  '7/20': { name: 'Apollinaris', type: 'Optional Memorial', quote: 'Christ is my life, and death is my gain.', intro: 'First bishop of Ravenna, martyred for the faith.' },
  '7/21': { name: 'Lawrence of Brindisi', type: 'Optional Memorial', quote: 'All for the honor and glory of God.', intro: 'Capuchin priest, Doctor of the Church, linguist and diplomat.' },
  '7/22': { name: 'Mary Magdalene', type: 'Feast', quote: 'I have seen the Lord!', intro: 'First witness to the Resurrection, apostle to the apostles.' },
  '7/23': { name: 'Bridget', type: 'Optional Memorial', quote: 'Be you a bright candle before men.', intro: 'Swedish mystic, mother of eight, founded the Brigittines.' },
  '7/24': { name: 'Sharbel Makhluf', type: 'Optional Memorial', quote: 'God is found in silence and solitude.', intro: 'Lebanese hermit monk known for miracles.' },
  '7/25': { name: 'James', type: 'Feast', quote: 'Can you drink the cup that I am to drink?', intro: 'Apostle, brother of John, first apostle to be martyred.' },
  '7/26': { name: 'Joachim and Anne', type: 'Memorial', quote: 'The fruit of your womb will be blessed.', intro: 'Parents of the Blessed Virgin Mary.' },
  '7/29': { name: 'Martha, Mary, and Lazarus', type: 'Memorial', quote: 'Lord, if you had been here, my brother would not have died.', intro: 'Friends of Jesus who showed hospitality and faith.' },
  '7/30': { name: 'Peter Chrysologus', type: 'Optional Memorial', quote: 'All that we have is Christ\'s; all that Christ has, he has made ours.', intro: 'Bishop of Ravenna, Doctor of the Church, golden-worded preacher.' },
  '7/31': { name: 'Ignatius of Loyola', type: 'Memorial', quote: 'Go forth and set the world on fire.', intro: 'Founded the Jesuits, author of Spiritual Exercises.' },

  // August
  '8/1': { name: 'Alphonsus Liguori', type: 'Memorial', quote: 'Those who love Mary are saved; those who love her much are saints.', intro: 'Founded Redemptorists, Doctor of the Church, moral theologian.' },
  '8/2': { name: 'Eusebius of Vercelli', type: 'Optional Memorial', quote: 'Where Christ is, there is no room for heresy.', intro: 'Bishop who fought Arianism and lived in community with his clergy.' },
  '8/4': { name: 'John Vianney', type: 'Memorial', quote: 'The priesthood is the love of the heart of Jesus.', intro: 'Curé of Ars, patron of parish priests, known for confessions and holiness.' },
  '8/5': { name: 'Dedication of St. Mary Major', type: 'Optional Memorial', quote: 'My house shall be called a house of prayer.', intro: 'Commemorates the dedication of the basilica in Rome.' },
  '8/6': { name: 'Transfiguration', type: 'Feast', quote: 'This is my beloved Son, with whom I am well pleased; listen to him.', intro: 'Celebrates Christ\'s divine glory revealed on Mount Tabor.' },
  '8/7': { name: 'Cajetan', type: 'Optional Memorial', quote: 'Seek first the kingdom of God and his righteousness.', intro: 'Founded the Theatines to reform clergy and serve the poor.' },
  '8/8': { name: 'Dominic', type: 'Memorial', quote: 'Arm yourself with prayer rather than a sword; wear humility rather than fine clothes.', intro: 'Founded the Dominicans (Order of Preachers) to combat heresy through preaching.' },
  '8/9': { name: 'Teresa Benedicta of the Cross', type: 'Optional Memorial', quote: 'The world doesn\'t need what women have, it needs what women are.', intro: 'Jewish philosopher who became Carmelite nun, died at Auschwitz.' },
  '8/10': { name: 'Lawrence', type: 'Feast', quote: 'Turn me over, I\'m done on this side!', intro: 'Deacon martyr who distributed Church treasures to the poor.' },
  '8/11': { name: 'Clare', type: 'Memorial', quote: 'We become what we love and who we love shapes what we become.', intro: 'Founded the Poor Clares, first woman to write a religious rule.' },
  '8/12': { name: 'Jane Frances de Chantal', type: 'Optional Memorial', quote: 'Jesus alone, and always Jesus.', intro: 'Widow who founded the Visitation Sisters with Francis de Sales.' },
  '8/13': { name: 'Pontian and Hippolytus', type: 'Optional Memorial', quote: 'In exile we found our unity in Christ.', intro: 'Pope and antipope reconciled in exile and martyrdom.' },
  '8/14': { name: 'Maximilian Kolbe', type: 'Memorial', quote: 'No one has greater love than this, to lay down one\'s life for one\'s friends.', intro: 'Franciscan priest who volunteered to die in place of another at Auschwitz.' },
  '8/15': { name: 'Assumption', type: 'Solemnity', quote: 'All generations will call me blessed.', intro: 'Celebrates Mary\'s assumption body and soul into heaven.' },
  '8/16': { name: 'Stephen of Hungary', type: 'Optional Memorial', quote: 'I offer this kingdom to the Virgin Mary.', intro: 'First Christian king of Hungary who established the Church there.' },
  '8/19': { name: 'John Eudes', type: 'Optional Memorial', quote: 'The heart of Jesus is the heart of God made man.', intro: 'Promoted devotion to Sacred Hearts of Jesus and Mary.' },
  '8/20': { name: 'Bernard', type: 'Memorial', quote: 'Mary is the neck through which the Head communicates with the body.', intro: 'Cistercian abbot, Doctor of the Church, preached the Second Crusade.' },
  '8/21': { name: 'Pius X', type: 'Memorial', quote: 'Restore all things in Christ.', intro: 'Pope who promoted frequent Communion and reformed liturgy.' },
  '8/22': { name: 'Queenship of Mary', type: 'Memorial', quote: 'Hail, holy Queen, Mother of Mercy.', intro: 'Celebrates Mary as Queen of Heaven and Earth.' },
  '8/23': { name: 'Rose of Lima', type: 'Optional Memorial', quote: 'Apart from the cross there is no other ladder by which we may get to heaven.', intro: 'First saint of the Americas, Dominican tertiary and mystic.' },
  '8/24': { name: 'Bartholomew', type: 'Feast', quote: 'Rabbi, you are the Son of God! You are the King of Israel!', intro: 'One of the twelve apostles, also called Nathanael, traditionally martyred in Armenia.' },
  '8/25': { name: 'Louis', type: 'Optional Memorial', quote: 'I would rather be a leper than commit a mortal sin.', intro: 'King of France who led two Crusades and cared for the poor.' },
  '8/27': { name: 'Monica', type: 'Memorial', quote: 'Nothing is far from God.', intro: 'Mother of Augustine who prayed for his conversion for years.' },
  '8/28': { name: 'Augustine', type: 'Memorial', quote: 'You have made us for yourself, O Lord, and our hearts are restless until they rest in you.', intro: 'Bishop of Hippo, Doctor of the Church, influential theologian and philosopher.' },
  '8/29': { name: 'Passion of John the Baptist', type: 'Memorial', quote: 'It is not lawful for you to have her.', intro: 'Commemorates the martyrdom of Christ\'s forerunner.' },

  // September
  '9/3': { name: 'Gregory the Great', type: 'Memorial', quote: 'He who would climb to a lofty height must go by steps, not leaps.', intro: 'Pope, Doctor of the Church, sent missionaries to England, reformed liturgy.' },
  '9/8': { name: 'Birth of Mary', type: 'Feast', quote: 'Rejoice, O highly favored daughter! The Lord is with you.', intro: 'Celebrates the birth of the Mother of God.' },
  '9/9': { name: 'Peter Claver', type: 'Memorial', quote: 'Slave of the slaves forever.', intro: 'Jesuit who ministered to African slaves in Colombia.' },
  '9/12': { name: 'Most Holy Name of Mary', type: 'Optional Memorial', quote: 'Blessed is the name of Mary.', intro: 'Honors the name of the Blessed Virgin Mary.' },
  '9/13': { name: 'John Chrysostom', type: 'Memorial', quote: 'Not to enable the poor to share in our goods is to steal from them.', intro: 'Archbishop of Constantinople, Doctor of the Church, "Golden-mouthed" preacher.' },
  '9/14': { name: 'Exaltation of the Holy Cross', type: 'Feast', quote: 'We should glory in the cross of our Lord Jesus Christ.', intro: 'Celebrates the Cross as the instrument of our salvation.' },
  '9/15': { name: 'Our Lady of Sorrows', type: 'Memorial', quote: 'O all you who pass by, see if there is any sorrow like my sorrow.', intro: 'Honors Mary\'s suffering as she stood by the Cross.' },
  '9/16': { name: 'Cornelius and Cyprian', type: 'Memorial', quote: 'Outside the Church there is no salvation.', intro: 'Pope and bishop who were martyred during persecution.' },
  '9/17': { name: 'Robert Bellarmine', type: 'Optional Memorial', quote: 'The mark of the true Church is that she suffers persecution.', intro: 'Jesuit cardinal, Doctor of the Church, defender of the faith.' },
  '9/19': { name: 'Januarius', type: 'Optional Memorial', quote: 'The blood of martyrs is the seed of Christians.', intro: 'Bishop of Naples whose blood liquefies miraculously.' },
  '9/20': { name: 'Andrew Kim Tae-gon and Companions', type: 'Memorial', quote: 'This is why I was born, and why I came into the world.', intro: 'Korean martyrs who died for the faith in the 19th century.' },
  '9/21': { name: 'Matthew', type: 'Feast', quote: 'Follow me.', intro: 'Tax collector who became apostle and evangelist.' },
  '9/23': { name: 'Padre Pio', type: 'Memorial', quote: 'Pray, hope, and don\'t worry.', intro: 'Capuchin priest with stigmata, known for confessions and miracles.' },
  '9/26': { name: 'Cosmas and Damian', type: 'Optional Memorial', quote: 'We take no payment but from God.', intro: 'Twin brothers, physicians who treated the poor without charge.' },
  '9/27': { name: 'Vincent de Paul', type: 'Memorial', quote: 'Charity is the cement which binds communities to God and persons to one another.', intro: 'Founded Vincentians and Daughters of Charity to serve the poor.' },
  '9/28': { name: 'Wenceslaus', type: 'Optional Memorial', quote: 'It is better to die for God than to live without him.', intro: 'Duke of Bohemia martyred by his brother.' },
  '9/29': { name: 'Michael, Gabriel, and Raphael', type: 'Feast', quote: 'Who is like God?', intro: 'The three archangels named in Scripture.' },
  '9/30': { name: 'Jerome', type: 'Memorial', quote: 'Ignorance of Scripture is ignorance of Christ.', intro: 'Translated the Bible into Latin (Vulgate), Doctor of the Church.' },

  // October
  '10/1': { name: 'Therese of the Child Jesus', type: 'Memorial', quote: 'I will spend my heaven doing good on earth.', intro: 'The Little Flower, Doctor of the Church, taught the "little way" of spiritual childhood.' },
  '10/2': { name: 'Guardian Angels', type: 'Memorial', quote: 'See that you do not despise one of these little ones, for their angels continually see the face of my Father.', intro: 'Celebrates the angels God assigns to protect each person.' },
  '10/4': { name: 'Francis of Assisi', type: 'Memorial', quote: 'Preach the Gospel at all times; when necessary, use words.', intro: 'Founded Franciscans, rebuilt the Church, patron of ecology and peace.' },
  '10/5': { name: 'Faustina Kowalska', type: 'Optional Memorial', quote: 'Jesus, I trust in You!', intro: 'Polish nun who received revelations of Divine Mercy.' },
  '10/6': { name: 'Bruno', type: 'Optional Memorial', quote: 'O Bonitas! O Goodness!', intro: 'Founded the Carthusians, strictest contemplative order.' },
  '10/7': { name: 'Our Lady of the Rosary', type: 'Memorial', quote: 'Hail Mary, full of grace, the Lord is with thee.', intro: 'Commemorates the victory at Lepanto through the Rosary.' },
  '10/9': { name: 'Denis and Companions', type: 'Optional Memorial', quote: 'Christ is our head, and we are his body.', intro: 'First bishop of Paris, martyred with companions.' },
  '10/11': { name: 'John XXIII', type: 'Optional Memorial', quote: 'Consult not your fears but your hopes and dreams.', intro: 'Good Pope John who convened Vatican II.' },
  '10/14': { name: 'Callistus I', type: 'Optional Memorial', quote: 'I have the power to forgive all sins.', intro: 'Pope who defended mercy for repentant sinners.' },
  '10/15': { name: 'Teresa of Avila', type: 'Memorial', quote: 'Let nothing disturb you, let nothing frighten you, all things pass away: God never changes.', intro: 'Reformed Carmelites, Doctor of the Church, mystic who taught stages of prayer.' },
 '10/16': { name: 'Hedwig', type: 'Optional Memorial', quote: 'All I have belongs to God and the poor.', intro: 'Duchess of Silesia who used wealth to serve the poor.' },
 '10/17': { name: 'Ignatius of Antioch', type: 'Memorial', quote: 'I am the wheat of God, and am ground by the teeth of wild beasts.', intro: 'Bishop and martyr who wrote seven letters while being taken to Rome for execution.' },
 '10/18': { name: 'Luke', type: 'Feast', quote: 'I have decided to write an orderly account for you.', intro: 'Evangelist, physician, companion of Paul, patron of artists and doctors.' },
 '10/19': { name: 'John de Brebeuf and Isaac Jogues', type: 'Memorial', quote: 'I came here to live for God alone.', intro: 'North American martyrs, Jesuits killed evangelizing Native Americans.' },
 '10/20': { name: 'Paul of the Cross', type: 'Optional Memorial', quote: 'Every pain of the crucified Jesus speaks of love.', intro: 'Founded the Passionists to preach Christ\'s Passion.' },
 '10/22': { name: 'John Paul II', type: 'Optional Memorial', quote: 'Do not be afraid. Open wide the doors for Christ.', intro: 'Pope who helped end communism, promoted Divine Mercy and Theology of the Body.' },
 '10/23': { name: 'John of Capistrano', type: 'Optional Memorial', quote: 'Those who hope in the Lord will never be defeated.', intro: 'Franciscan preacher who rallied Christians against Turkish invasion.' },
 '10/24': { name: 'Anthony Mary Claret', type: 'Optional Memorial', quote: 'My glory is the Cross of Christ.', intro: 'Archbishop of Cuba, founded Claretians.' },
 '10/28': { name: 'Simon and Jude', type: 'Feast', quote: 'Build yourselves up in your most holy faith.', intro: 'Two of the twelve apostles, traditionally martyred together.' },

 // November
 '11/1': { name: 'All Saints', type: 'Solemnity', quote: 'Blessed are they who are called to the wedding feast of the Lamb.', intro: 'Celebrates all the saints in heaven, known and unknown.' },
 '11/2': { name: 'All Souls', type: 'Commemoration', quote: 'Eternal rest grant unto them, O Lord.', intro: 'Day of prayer for all the faithful departed.' },
 '11/3': { name: 'Martin de Porres', type: 'Optional Memorial', quote: 'Compassion, my dear brother, is preferable to cleanliness.', intro: 'Dominican brother, patron of social justice and racial harmony.' },
 '11/4': { name: 'Charles Borromeo', type: 'Memorial', quote: 'We were born to work together like feet, hands, and eyes.', intro: 'Cardinal Archbishop of Milan who reformed the Church after Trent.' },
 '11/9': { name: 'Dedication of the Lateran Basilica', type: 'Feast', quote: 'This is the house of God and the gate of heaven.', intro: 'Mother church of all churches, the Pope\'s cathedral.' },
 '11/10': { name: 'Leo the Great', type: 'Memorial', quote: 'Christian, recognize your dignity.', intro: 'Pope and Doctor who defended doctrine and saved Rome from Attila.' },
 '11/11': { name: 'Martin of Tours', type: 'Memorial', quote: 'I am Christ\'s soldier; I am not allowed to fight.', intro: 'Soldier who shared his cloak with a beggar, became bishop.' },
 '11/12': { name: 'Josaphat', type: 'Memorial', quote: 'If I could give my life to unite the Churches, I would gladly do so.', intro: 'Bishop and martyr who worked for unity between East and West.' },
 '11/13': { name: 'Frances Xavier Cabrini', type: 'Memorial', quote: 'We must pray without tiring, for the prayer of the just man can do much.', intro: 'First American citizen to be canonized, patron of immigrants.' },
 '11/15': { name: 'Albert the Great', type: 'Optional Memorial', quote: 'Wonder is the beginning of wisdom.', intro: 'Dominican bishop, Doctor of the Church, teacher of Aquinas, scientist.' },
 '11/16': { name: 'Margaret of Scotland', type: 'Optional Memorial', quote: 'Christ poor and crucified should be the model for all Christians.', intro: 'Queen who reformed the Church in Scotland.' },
 '11/17': { name: 'Elizabeth of Hungary', type: 'Memorial', quote: 'How can I wear a crown of gold when the Lord Jesus wears a crown of thorns?', intro: 'Princess who gave away her wealth to serve the poor.' },
 '11/18': { name: 'Dedication of Basilicas of Peter and Paul', type: 'Optional Memorial', quote: 'You are fellow citizens with the saints and members of the household of God.', intro: 'Commemorates the dedication of the two apostles\' basilicas in Rome.' },
 '11/21': { name: 'Presentation of Mary', type: 'Memorial', quote: 'Here I am, the servant of the Lord.', intro: 'Commemorates Mary\'s presentation in the Temple as a child.' },
 '11/22': { name: 'Cecilia', type: 'Memorial', quote: 'Sing to the Lord a new song.', intro: 'Virgin martyr, patron of musicians.' },
 '11/23': { name: 'Clement I', type: 'Optional Memorial', quote: 'Let us be humble minded and put aside all arrogance, pride and foolishness.', intro: 'Fourth Pope, disciple of Peter, martyred by drowning.' },
 '11/24': { name: 'Andrew Dung-Lac and Companions', type: 'Memorial', quote: 'I die for God, and it is a joyful thing.', intro: 'Vietnamese martyrs spanning three centuries.' },
 '11/25': { name: 'Catherine of Alexandria', type: 'Optional Memorial', quote: 'I am the bride of Christ.', intro: 'Virgin martyr, one of the Fourteen Holy Helpers.' },
 '11/30': { name: 'Andrew', type: 'Feast', quote: 'Come and see!', intro: 'First-called apostle, brother of Peter, patron of Scotland.' },

 // December
 '12/3': { name: 'Francis Xavier', type: 'Memorial', quote: 'Give me the children until they are seven and anyone may have them afterwards.', intro: 'Jesuit missionary to Asia, patron of foreign missions.' },
 '12/4': { name: 'John Damascene', type: 'Optional Memorial', quote: 'An image is a song of triumph, a revelation, an enduring monument to the victory of the saints.', intro: 'Doctor of the Church who defended sacred images.' },
 '12/6': { name: 'Nicholas', type: 'Optional Memorial', quote: 'The good must associate with the good.', intro: 'Bishop of Myra known for generosity, basis for Santa Claus.' },
 '12/7': { name: 'Ambrose', type: 'Memorial', quote: 'When in Rome, do as the Romans do.', intro: 'Bishop of Milan, Doctor of the Church, converted Augustine.' },
 '12/8': { name: 'Immaculate Conception', type: 'Solemnity', quote: 'I am the Immaculate Conception.', intro: 'Celebrates Mary\'s conception without original sin.' },
 '12/9': { name: 'Juan Diego', type: 'Optional Memorial', quote: 'Am I not here, who am your Mother?', intro: 'Visionary of Our Lady of Guadalupe.' },
 '12/11': { name: 'Damasus I', type: 'Optional Memorial', quote: 'The Word of God must be given to all people.', intro: 'Pope who commissioned Jerome to translate the Bible.' },
 '12/12': { name: 'Our Lady of Guadalupe', type: 'Feast', quote: 'Do not be afraid. Am I not your Mother?', intro: 'Patroness of the Americas, appeared to Juan Diego.' },
 '12/13': { name: 'Lucy', type: 'Memorial', quote: 'The light of Christ shines in me.', intro: 'Virgin martyr, patron of the blind, name means light.' },
 '12/14': { name: 'John of the Cross', type: 'Memorial', quote: 'In the evening of life, we will be judged on love alone.', intro: 'Carmelite mystic, Doctor of the Church, wrote Dark Night of the Soul.' },
 '12/21': { name: 'Peter Canisius', type: 'Optional Memorial', quote: 'If you want to help souls, you must teach the catechism.', intro: 'Jesuit who helped preserve Catholicism in Germany.' },
 '12/23': { name: 'John of Kanty', type: 'Optional Memorial', quote: 'Fight all error, but do it with good humor, patience, kindness, and love.', intro: 'Polish priest and professor known for charity.' },
 '12/25': { name: 'Nativity of the Lord', type: 'Solemnity', quote: 'For unto you is born this day a Savior, who is Christ the Lord.', intro: 'Christmas, the birth of Jesus Christ our Savior.' },
 '12/26': { name: 'Stephen', type: 'Feast', quote: 'Lord, do not hold this sin against them.', intro: 'First Christian martyr, one of the seven deacons.' },
 '12/27': { name: 'John', type: 'Feast', quote: 'Little children, love one another.', intro: 'Beloved disciple, apostle and evangelist.' },
 '12/28': { name: 'Holy Innocents', type: 'Feast', quote: 'A voice was heard in Ramah, weeping and great mourning.', intro: 'Children killed by Herod in his attempt to kill the infant Jesus.' },
 '12/29': { name: 'Thomas Becket', type: 'Optional Memorial', quote: 'I am ready to die for my Lord.', intro: 'Archbishop of Canterbury martyred in his cathedral.' },
 '12/31': { name: 'Sylvester I', type: 'Optional Memorial', quote: 'Let us rejoice in the Lord, for He has given us peace.', intro: 'Pope during Constantine\'s reign when Christianity became legal.' }
};

// Get the saint/feast for the current date
const currentDate = dateInfo.monthDay || dateInfo;
const todaySaint = saintsDatabase[currentDate] || null;

// If no specific saint, check if it's a Sunday
let result = {};
if (todaySaint) {
 result = todaySaint;
} else if (dateInfo.dayOfWeek === 'Sunday') {
 // Calculate which Sunday of Ordinary Time or special season
 result = {
   name: `${dateInfo.dayOfWeek} in Ordinary Time`,
   type: 'Sunday',
   intro: 'The Lord\'s Day, when we gather as a community to celebrate the Eucharist and commemorate Christ\'s resurrection.'
 };
}

// Add Wikipedia link helper
if (result.name) {
 result.wikipediaLink = `https://en.wikipedia.org/wiki/${result.name.replace(/ /g, '_').replace(',', '')}`;
}

console.log(`Saint/Feast for ${currentDate}:`, result.name || 'No specific feast');

return result;
